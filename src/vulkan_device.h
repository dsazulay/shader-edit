#pragma once

#include "window.h"

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <cstddef>

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;
    auto isComplete() -> bool 
    {
        return graphicsFamilyHasValue && presentFamilyHasValue;
    }
};

class VulkanDevice {
public:
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    VulkanDevice(Window& window);
    ~VulkanDevice();
    VulkanDevice(const VulkanDevice& other) = delete;
    VulkanDevice(VulkanDevice&& other) = delete;
    auto operator=(const VulkanDevice& other) -> VulkanDevice& = delete;
    auto operator=(VulkanDevice&& other) -> VulkanDevice& = delete;

    auto getCommandPool() -> VkCommandPool { return commandPool; }
    auto device() -> VkDevice { return device_; }
    auto surface() -> VkSurfaceKHR { return surface_; }
    auto graphicsQueue() -> VkQueue { return graphicsQueue_; }
    auto presentQueue() -> VkQueue { return presentQueue_; }

    auto getSwapChainSupport() -> SwapChainSupportDetails { return querySwapChainSupport(physicalDevice); }
    auto findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) -> uint32_t;
    auto findPhysicalQueueFamilies() -> QueueFamilyIndices { return findQueueFamilies(physicalDevice); }
    auto findSupportedFormat(
            const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) -> VkFormat;

    // Buffer Helper Functions
    auto createBuffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer &buffer,
            VkDeviceMemory &bufferMemory) -> void;
    auto beginSingleTimeCommands() -> VkCommandBuffer;
    auto endSingleTimeCommands(VkCommandBuffer commandBuffer) -> void;
    auto copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) -> void;
    auto copyBufferToImage(
            VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) -> void;

    auto createImageWithInfo(
            const VkImageCreateInfo &imageInfo,
            VkMemoryPropertyFlags properties,
            VkImage &image,
            VkDeviceMemory &imageMemory) -> void;

    VkPhysicalDeviceProperties properties;

 private:
    auto createInstance() -> void;
    auto setupDebugMessenger() -> void;
    auto createSurface() -> void;
    auto pickPhysicalDevice() -> void;
    auto createLogicalDevice() -> void;
    auto createCommandPool() -> void;

    // helper functions
    auto isDeviceSuitable(VkPhysicalDevice device) -> bool;
    auto getRequiredExtensions() -> std::vector<const char *>;
    auto checkValidationLayerSupport() -> bool;
    auto findQueueFamilies(VkPhysicalDevice device) -> QueueFamilyIndices;
    auto populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) -> void;
    auto hasGflwRequiredInstanceExtensions() -> void;
    auto checkDeviceExtensionSupport(VkPhysicalDevice device) -> bool;
    auto querySwapChainSupport(VkPhysicalDevice device) -> SwapChainSupportDetails;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    Window& window;
    VkCommandPool commandPool;

    VkDevice device_;
    VkSurfaceKHR surface_;
    VkQueue graphicsQueue_;
    VkQueue presentQueue_;

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

