#pragma once

#include "includes/lodepng/lodepng.h"

#include <array>
#include <cstdint>
#include <expected>
#include <span>
#include <string>
#include <vector>

using Rgb = std::array<std::uint8_t, 3>;

// 8-bit RGB pixels, row-major.
struct Image {
    int width = 0;
    int height = 0;
    std::vector<std::uint8_t> pixels;

    Rgb at(int x, int y) const {
        const auto i = (static_cast<std::size_t>(y) * width + x) * 3;
        return {pixels[i], pixels[i + 1], pixels[i + 2]};
    }
};

// Blends `from` on the left edge into `to` on the right edge.
inline Image make_gradient(int width, int height, Rgb from, Rgb to) {
    Image image{width, height, {}};
    image.pixels.reserve(static_cast<std::size_t>(width) * height * 3);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (std::size_t c = 0; c < 3; ++c) {
                const int blended = from[c] + (to[c] - from[c]) * x / (width - 1);
                image.pixels.push_back(static_cast<std::uint8_t>(blended));
            }
        }
    }

    return image;
}

inline Rgb average(const Image& image) {
    std::array<std::size_t, 3> sums{};

    for (std::size_t i = 0; i < image.pixels.size(); ++i) {
        sums[i % 3] += image.pixels[i];
    }

    const std::size_t count = image.pixels.size() / 3;
    return {
        static_cast<std::uint8_t>(sums[0] / count),
        static_cast<std::uint8_t>(sums[1] / count),
        static_cast<std::uint8_t>(sums[2] / count),
    };
}

// Encodes to PNG in memory rather than to a file. LodePNG returns 0 on
// success, or an error code that lodepng_error_text() describes.
inline std::expected<std::vector<std::uint8_t>, std::string> encode_png(const Image& image) {
    const auto width = static_cast<unsigned>(image.width);
    const auto height = static_cast<unsigned>(image.height);
    std::vector<std::uint8_t> png;

    if (const unsigned error = lodepng::encode(png, image.pixels, width, height, LCT_RGB)) {
        return std::unexpected(lodepng_error_text(error));
    }

    return png;
}

inline std::expected<Image, std::string> decode_png(std::span<const std::uint8_t> png) {
    std::vector<std::uint8_t> pixels;
    unsigned width = 0;
    unsigned height = 0;

    // LCT_RGB: decode to 3 bytes per pixel, whatever the PNG stores.
    if (const unsigned error = lodepng::decode(pixels, width, height, png.data(), png.size(), LCT_RGB)) {
        return std::unexpected(lodepng_error_text(error));
    }

    return Image{static_cast<int>(width), static_cast<int>(height), std::move(pixels)};
}
