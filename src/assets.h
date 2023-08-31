#pragma once

#include "vulkan_device.h"

#include "vulkan/vulkan_core.h"
#include <string>
#include <string_view>
#include <filesystem>
#include <vector>

class Shader
{
public:
    auto compile(std::string_view vertexSrc, std::string_view fragSrc) -> void;
    auto use() const -> void;
    auto setUniformBlock(std::string_view blockName, int blockID) -> void;
    auto setInt(std::string_view name, int value) const -> void;
    auto setVec2(std::string_view name, float x, float y) const -> void;
    auto getVertCode() -> std::vector<char>&;
    auto getFragCode() -> std::vector<char>&;

    std::vector<char> m_vertCode;
    std::vector<char> m_fragCode;

    std::string vertFilePath;
    std::string fragFilePath;
    std::filesystem::file_time_type vertLastWriteTime;
    std::filesystem::file_time_type fragLastWriteTime;

private:
    unsigned int ID;

    constexpr static int logBufferSize = 512;
};

class Texture
{
public:
    auto load(std::string_view path, VkFormat format, VulkanDevice& device) -> void;
    auto free(VulkanDevice& device) -> void;
    auto imageView() -> VkImageView;
    auto sampler() -> VkSampler;

private:
    auto createImage(unsigned char* pPixels, VkImageCreateInfo imageInfo, VkDeviceSize imageSize, VulkanDevice& device) -> void;
    auto createImageView(VkFormat format, VulkanDevice& device) -> void;
    auto createSampler(VulkanDevice& device) -> void;

    VkImage m_image;
    VkDeviceMemory m_imageMemory;
    VkImageView m_imageView;
    VkSampler m_sampler;
};
