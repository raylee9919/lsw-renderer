/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */




    


#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>

#include "core.h"
#include "intrinsics.h"
#include "math.h"
#include "platform.h"
#include "dst.h"

Os os;

#include "renderer.h"
global Renderer renderer;

#include "linux_renderer.h"
#include "renderer_vulkan.cpp"


// @SPEC: ZII
function
linux_alloc(size_t size) {
    void *result = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    return result;
}

function
linux_free(void *memory) {
    if (memory) {
        // @TODO:
    }
}

function void
linux_vk_create_instance(Vulkan *vk, const char **layers, u32 layer_count) {
    VkApplicationInfo app_info{};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = "Hello World";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = "No Engine";
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = VK_API_VERSION_1_0;

    const char *extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
    };

    VkInstanceCreateInfo create_info{};
    create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo        = &app_info;
    create_info.enabledExtensionCount   = arraycount(extensions);
    create_info.ppEnabledExtensionNames = extensions;
    create_info.enabledLayerCount       = layer_count;
    create_info.ppEnabledLayerNames     = layers;

    ASSERT(vkCreateInstance(&create_info, 0, &vk->instance) == VK_SUCCESS);
}

function VkSurfaceKHR
linux_vk_create_surface(VkInstance instance, Display *display, Window window) {
    VkSurfaceKHR result{};

    VkXlibSurfaceCreateInfoKHR create_info{};
    create_info.sType       = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    create_info.pNext       = 0;
    create_info.flags       = 0;
    create_info.dpy         = display;
    create_info.window      = window;

    ASSERT(vkCreateXlibSurfaceKHR(instance, &create_info, 0, &result) == VK_SUCCESS);

    return result;
}

function b32
linux_vk_pick_physical_device_and_create_surface(Vulkan *vk, Display *display, Window window,
                                                 VkPhysicalDevice *physical_devices, u32 physical_device_count) {
    b32 result = false;

    for (u32 pdi = 0; pdi < physical_device_count; ++pdi) {
        VkPhysicalDevice physical_device = physical_devices[pdi];

        VkSurfaceKHR surface = linux_vk_create_surface(vk->instance, display, window);

        u32 queue_family_property_count;
        VkQueueFamilyProperties *queue_family_properties;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_property_count, 0);
        queue_family_properties = (VkQueueFamilyProperties *)os.alloc(sizeof(queue_family_properties) * queue_family_property_count);
        SCOPE_EXIT(os.free(queue_family_properties));
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_property_count, queue_family_properties);

        Vk_Queue_Family graphics_queue_family{};
        b32 graphics_queue_family_found = false;

        Vk_Queue_Family present_queue_family{};
        b32 present_queue_family_found = false;

        // @TODO: Metric for picking good queue family?
        for (u32 qfi = 0; qfi < queue_family_property_count; ++qfi) {
            VkQueueFamilyProperties queue_family_property = queue_family_properties[qfi];
            VkQueueFlags flags = queue_family_property.queueFlags;
            if (flags & VK_QUEUE_GRAPHICS_BIT) {
                graphics_queue_family.index = qfi;
                graphics_queue_family.queue_count = queue_family_property.queueCount;
                graphics_queue_family_found = true;
            }

            VkBool32 present_supported;
            ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, qfi, surface, &present_supported) == VK_SUCCESS);

            if (present_supported && vkGetPhysicalDeviceXlibPresentationSupportKHR(physical_device, qfi)) {
                present_queue_family.index = qfi;
                present_queue_family.queue_count = queue_family_property.queueCount;
                present_queue_family_found = true;
            }
        }

        // @NOTE: If criteria is met, we'll stop iterating through physical devices.
        if (graphics_queue_family_found && present_queue_family_found) {
            vk->physical_device       = physical_device;
            vk->surface               = surface;
            vk->graphics_queue_family = graphics_queue_family;
            vk->present_queue_family  = present_queue_family;

            result = true;
            break;
        } else {
            vkDestroySurfaceKHR(vk->instance, surface, 0);
        }
    }

    return result;
}

function void
linux_vk_pick_best_physical_device_and_create_surface(Vulkan *vk, Display *display, Window window) {
    u32 physical_device_count = vk_query_physical_device_count(vk->instance);
    VkPhysicalDevice *physical_devices = (VkPhysicalDevice *)linux_alloc(sizeof(VkPhysicalDevice)*physical_device_count);
    SCOPE_EXIT(linux_free(physical_devices));
    vk_query_physical_devices(vk->instance, physical_devices);
    vk_sort_physical_devices(physical_devices, physical_device_count);
    ASSERT(linux_vk_pick_physical_device_and_create_surface(vk, display, window, physical_devices, physical_device_count));
}

RENDERER_END_FRAME(linux_end_frame) {
    Window window = ((Renderer_Linux *)renderer.platform)->window;

    RECT rect{};
    GetClientRect(hwnd, &rect);
    u32 width  = rect.right  - rect.left;
    u32 height = rect.bottom - rect.top;

    if (width > 0 && height > 0) {
        Vulkan *vk = (Vulkan *)renderer.backend;

        if (vk->swapchain_image_extent.width != width || vk->swapchain_image_extent.height != height) {
            vkQueueWaitIdle(vk->graphics_queue);
            vk_recreate_swapchain(vk, width, height);
        }

        vk_draw(vk);
    }

    renderer.sort_keys.clear();
    renderer.vertices.clear();
    renderer.drawcall_vertex_count.clear();
}

LINUX_LOAD_RENDERER(linux_load_renderer) {
    os.alloc = linux_alloc;
    os.free  = linux_free;

    renderer.platform = linux_alloc(sizeof(Renderer_Linux));
    Renderer_Linux *renderer_linux = (Renderer_Linux *)renderer.platform;
    renderer_linux->display = display;
    renderer_linux->window = window;

    renderer.backend = linux_alloc(sizeof(Vulkan));
    Vulkan *vk = (Vulkan *)renderer.backend;

    renderer.image_hash_table.init(32);


    const char *desired_layers[] = {
#if __DEBUG
        "VK_LAYER_KHRONOS_validation",
        "VK_LAYER_KHRONOS_profiles",
#endif
    };
    u32 available_layer_count = vk_query_available_layer_count();
    VkLayerProperties *available_layers = (VkLayerProperties *)linux_alloc(sizeof(VkLayerProperties) * available_layer_count);
    SCOPE_EXIT(linux_free(available_layers));
    vk_query_available_layers(available_layers);
    for (u32 i = 0; i < arraycount(desired_layers); ++i) {
        if (!vk_is_layer_available(desired_layers[i], available_layers, available_layer_count)) {
            ASSERT(!"Layer Not Found");
        }
    }

    linux_vk_create_instance(vk, desired_layers, arraycount(desired_layers));
    linux_vk_pick_best_physical_device_and_create_surface(vk, hwnd, hinst);

    vk_init(vk);

    return &renderer;
}
