#pragma once

// SDL_vulkan.h declares its own VkInstance/VkSurfaceKHR unless vulkan.h has
// been included first; include it first so there's only one definition.
#include <vulkan/vulkan.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <cstdint>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>

// Owns SDL_Init()/SDL_Quit() for the lifetime of the program.
class SdlContext {
public:
    SdlContext() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error(std::string("SDL_Init failed (") + SDL_GetError() + ")");
        }
    }

    ~SdlContext() { SDL_Quit(); }

    SdlContext(const SdlContext&) = delete;
    SdlContext& operator=(const SdlContext&) = delete;

    // "wayland", "x11", "windows", ...
    static std::string_view video_driver() { return SDL_GetCurrentVideoDriver(); }

    // Instance extensions the platform needs to present to a window. SDL only
    // knows them once Vulkan is loaded, which creating a Vulkan window does.
    static std::span<const char* const> required_vulkan_extensions() {
        std::uint32_t count = 0;
        const char* const* names = SDL_Vulkan_GetInstanceExtensions(&count);

        if (!names) {
            throw std::runtime_error(std::string("SDL found no Vulkan support (") + SDL_GetError() + ")");
        }

        return {names, count};
    }
};

using Window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;

// A resizable window that Vulkan can draw into.
inline Window make_vulkan_window(int width, int height, const char* title, bool visible) {
    SDL_WindowFlags flags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE;

    if (!visible) {
        flags |= SDL_WINDOW_HIDDEN;
    }

    Window window(SDL_CreateWindow(title, width, height, flags), &SDL_DestroyWindow);

    if (!window) {
        throw std::runtime_error(std::string("SDL_CreateWindow failed (") + SDL_GetError() + ")");
    }

    return window;
}
