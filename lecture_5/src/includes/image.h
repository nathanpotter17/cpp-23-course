#pragma once

#include "includes/lodepng/lodepng.h"

#include <cstdint>
#include <expected>
#include <filesystem>
#include <string>
#include <vector>

// 8-bit RGB pixels, row-major: 3 bytes per pixel.
struct Image {
    int width = 0;
    int height = 0;
    std::vector<std::uint8_t> pixels;

    std::uint8_t* at(int x, int y) {
        return pixels.data() + (static_cast<std::size_t>(y) * width + x) * 3;
    }

    const std::uint8_t* at(int x, int y) const {
        return pixels.data() + (static_cast<std::size_t>(y) * width + x) * 3;
    }
};

// Red increases left to right, green top to bottom, blue is constant.
inline Image make_gradient(int width, int height) {
    Image image{width, height, std::vector<std::uint8_t>(static_cast<std::size_t>(width) * height * 3)};

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::uint8_t* pixel = image.at(x, y);
            pixel[0] = static_cast<std::uint8_t>(x * 255 / (width - 1));
            pixel[1] = static_cast<std::uint8_t>(y * 255 / (height - 1));
            pixel[2] = 128;
        }
    }

    return image;
}

// LodePNG returns 0 on success, or an error code that lodepng_error_text() describes.
// LCT_RGB: our pixels are 3 bytes each.
inline std::expected<void, std::string> write_png(const Image& image, const std::filesystem::path& path) {
    const auto width = static_cast<unsigned>(image.width);
    const auto height = static_cast<unsigned>(image.height);

    if (const unsigned error = lodepng::encode(path.string(), image.pixels, width, height, LCT_RGB)) {
        return std::unexpected(path.string() + ": " + lodepng_error_text(error));
    }

    return {};
}

inline std::expected<Image, std::string> load_png(const std::filesystem::path& path) {
    std::vector<std::uint8_t> pixels;
    unsigned width = 0;
    unsigned height = 0;

    // LCT_RGB: decode to 3 bytes per pixel, whatever the file stores.
    if (const unsigned error = lodepng::decode(pixels, width, height, path.string(), LCT_RGB)) {
        return std::unexpected(path.string() + ": " + lodepng_error_text(error));
    }

    return Image{static_cast<int>(width), static_cast<int>(height), std::move(pixels)};
}
