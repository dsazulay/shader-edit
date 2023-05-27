#pragma once

#include "vulkan_device.h"

#include <OpenGL/OpenGL.h>
#include <vector>

class Model
{
public:
    struct Vector2
    {
        float x;
        float y;
    };

    struct Vertex
    {
        Vector2 position;
        Vector2 texCoord;

        static auto getBindingDescriptions() -> std::vector<VkVertexInputBindingDescription>;
        static auto getAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>;
    };

    Model(VulkanDevice& device, const std::vector<Vertex>& vertices);
    ~Model();
    Model(const Model& other) = delete;
    Model(Model&& other) = delete;
    auto operator=(const Model& other) -> Model& = delete;
    auto operator=(Model&& other) -> Model& = delete;

    auto bind(VkCommandBuffer commandBuffer) -> void;
    auto draw(VkCommandBuffer commandBuffer) -> void;

private:
    auto createVertexBuffers(const std::vector<Vertex>& vertices) -> void;

    VulkanDevice& m_vulkanDevice;
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    uint32_t m_vertexCount;
};
