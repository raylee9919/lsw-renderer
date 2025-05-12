/* ========================================================================

   (C) Copyright 2025 by Sung Woo Lee, All Rights Reserved.

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   ======================================================================== */

struct Vk_Queue_Family {
    u32 index;
    u32 queue_count;
};

struct Vk_Image_Unit {
    VkImage image;
    VkDeviceMemory memory;
    VkImageView view;
};

struct Vulkan {
    VkInstance instance;

    VkPhysicalDevice physical_device;
    VkPhysicalDeviceFeatures2 physical_device_features;
    VkPhysicalDeviceProperties physical_device_properties;
    VkPhysicalDeviceMemoryProperties memory_properties;

    VkSurfaceKHR surface;
    VkDevice device;

    VkPresentModeKHR present_mode;

    // @TODO: Single queue?
    Vk_Queue_Family graphics_queue_family;
    VkQueue graphics_queue;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;

    Vk_Queue_Family present_queue_family;
    VkQueue present_queue;

    u32 swapchain_image_count;
    VkExtent2D swapchain_image_extent;
    VkFormat swapchain_image_format;
    VkColorSpaceKHR swapchain_image_color_space;
    VkSwapchainKHR swapchain;
    VkImage *swapchain_images;
    VkImageView *swapchain_image_views;
    VkFramebuffer *swapchain_framebuffers;

    VkImage depth_image;
    VkImageView depth_image_view;
    VkDeviceMemory depth_image_memory;

    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_memory;

    VkBuffer index_buffer;
    VkDeviceMemory index_buffer_memory;

    VkRenderPass render_pass;
    VkPipelineLayout pipeline_layout;
    VkPipeline simple_pipeline;

    VkSemaphore image_available_semaphore;
    VkSemaphore render_finished_semaphore;
    VkFence in_flight_fence;

    VkBuffer uniform_buffer;
    VkDeviceMemory uniform_buffer_memory;
    void *uniform_buffer_mapped;

    VkDescriptorSet descriptor_set;


    VkImage DEBUG_image;
    VkDeviceMemory DEBUG_image_memory;
    VkImageView DEBUG_texture_image_view;
    VkSampler DEBUG_texture_sampler;

    Hash_Table<u32, Vk_Image_Unit> image_hash_table;
};
