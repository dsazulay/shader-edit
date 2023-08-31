#include "assets.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

auto Shader::compile(std::string_view vertexSrc, std::string_view fragSrc) -> void
{
    /*
    unsigned int vertexID{}, fragID{};
    int success{};
    std::string infoLog{};


    vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexID, 1, &vertexSrc, nullptr);
    glCompileShader(vertexID);

    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexID, Shader::logBufferSize, nullptr, infoLog.data());
        fmt::print("Vertex shader compilation failed: {}\n", infoLog);
    }

    fragID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragID, 1, &fragSrc, nullptr);
    glCompileShader(fragID);

    glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragID, Shader::logBufferSize, nullptr, infoLog.data());
        fmt::print("Fragment shader compilation failed: {}\n", infoLog);
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragID);

    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, Shader::logBufferSize, nullptr, infoLog.data());
        fmt::print("Shader program linking failed: {}\n", infoLog);
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragID);
    */
}

auto Shader::use() const -> void
{
    //glUseProgram(ID);
}

auto Shader::setUniformBlock(std::string_view blockName, int blockID) -> void
{
    //glUniformBlockBinding(ID, glGetUniformBlockIndex(ID, blockName), blockID);
}

auto Shader::setInt(std::string_view name, int value) const -> void
{
    //glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

auto Shader::setVec2(std::string_view name, float x, float y) const -> void
{
    //glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

auto Texture::load(std::string_view path, VkFormat format, VulkanDevice& device) -> void
{
    stbi_set_flip_vertically_on_load(true);
    int texWidth, texHeight, texChannels;
    unsigned char* pPixels = stbi_load(path.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pPixels)
    {
        throw std::runtime_error("failed to load texture image!");
    }

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = texWidth;
    imageInfo.extent.height = texHeight;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    createImage(pPixels, imageInfo, imageSize, device);
    stbi_image_free(pPixels);

    createImageView(format, device);
    createSampler(device);
}

auto Texture::createImage(unsigned char* pPixels, VkImageCreateInfo imageInfo, VkDeviceSize imageSize, VulkanDevice& device) -> void
{
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    device.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device.device(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pPixels, static_cast<uint32_t>(imageSize));
    vkUnmapMemory(device.device(), stagingBufferMemory);


    device.createImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image, m_imageMemory);

    device.transitionImageLayout(m_image, imageInfo.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    device.copyBufferToImage(stagingBuffer, m_image, imageInfo.extent.width, imageInfo.extent.height, 1);
    device.transitionImageLayout(m_image, imageInfo.format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
    vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
}

auto Texture::createImageView(VkFormat format, VulkanDevice& device) -> void
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device.device(), &viewInfo, nullptr, &m_imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }
}

auto Texture::createSampler(VulkanDevice& device) -> void
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;

    samplerInfo.maxAnisotropy = device.properties.limits.maxSamplerAnisotropy;

    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(device.device(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

auto Texture::free(VulkanDevice& device) -> void
{
    vkDestroySampler(device.device(), m_sampler, nullptr);
    vkDestroyImageView(device.device(), m_imageView, nullptr);
    vkDestroyImage(device.device(), m_image, nullptr);
    vkFreeMemory(device.device(), m_imageMemory, nullptr);
}

auto Texture::imageView() -> VkImageView
{
    return m_imageView;
}

auto Texture::sampler() -> VkSampler
{
    return m_sampler;
}
