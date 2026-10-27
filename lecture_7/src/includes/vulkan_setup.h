#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "includes/sdl.h"

#include <cstdint>
#include <optional>
#include <span>

// A GPU plus the queue family we'll use for both graphics and presenting.
struct GpuChoice {
    vk::raii::PhysicalDevice device;
    std::uint32_t queue_family;
};

// True if VK_LAYER_KHRONOS_validation is installed (it ships with the SDK,
// or `sudo apt install vulkan-validationlayers`).
bool validation_layer_available(const vk::raii::Context& context);

// `extensions` are the instance extensions to enable (e.g. from SDL).
// With `validation`, also enables the validation layer and VK_EXT_debug_utils.
vk::raii::Instance create_instance(
    const vk::raii::Context& context,
    std::span<const char* const> extensions,
    bool validation
);

// Prints validation warnings and errors to stderr. Needs an instance
// created with `validation`.
vk::raii::DebugUtilsMessengerEXT create_debug_messenger(const vk::raii::Instance& instance);

vk::raii::SurfaceKHR create_surface(const vk::raii::Instance& instance, SDL_Window* window);

// Prints every GPU, then picks a Vulkan 1.3 one that can draw and present to
// `surface`, preferring discrete over integrated over everything else.
std::optional<GpuChoice> pick_gpu(const vk::raii::Instance& instance, const vk::raii::SurfaceKHR& surface);

// A logical device with one queue from `gpu.queue_family`, VK_KHR_swapchain,
// and the Vulkan 1.3 dynamicRendering and synchronization2 features on.
vk::raii::Device create_device(const GpuChoice& gpu);
