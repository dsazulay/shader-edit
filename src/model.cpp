#include "model.h"

#include <cassert>

Model::Model(VulkanDevice& device, const std::vector<Vertex>& vertices) : m_vulkanDevice(device)
{
    createVertexBuffers(vertices);
}

Model::~Model()
{
    vkDestroyBuffer(m_vulkanDevice.device(), m_vertexBuffer, nullptr);
    vkFreeMemory(m_vulkanDevice.device(), m_vertexBufferMemory, nullptr);
}

auto Model::createVertexBuffers(const std::vector<Vertex>& vertices) -> void
{
    m_vertexCount = static_cast<uint32_t>(vertices.size());
    assert(m_vertexCount >= 3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices.at(0)) * m_vertexCount;
    m_vulkanDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            m_vertexBuffer, m_vertexBufferMemory);

    void* data;
    vkMapMemory(m_vulkanDevice.device(), m_vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(m_vulkanDevice.device(), m_vertexBufferMemory);
}

auto Model::bind(VkCommandBuffer commandBuffer) -> void
{
    VkBuffer buffers[] = {m_vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

auto Model::draw(VkCommandBuffer commandBuffer) -> void
{
    vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
}

auto Model::Vertex::getBindingDescriptions() -> std::vector<VkVertexInputBindingDescription>
{
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions.at(0).binding = 0;
    bindingDescriptions.at(0).stride = sizeof(Vertex);
    bindingDescriptions.at(0).inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

auto Model::Vertex::getAttributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    attributeDescriptions.at(0).binding = 0;
    attributeDescriptions.at(0).location = 0;
    attributeDescriptions.at(0).format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions.at(0).offset = 0;
    
    attributeDescriptions.at(1).binding = 0;
    attributeDescriptions.at(1).location = 1;
    attributeDescriptions.at(1).format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions.at(1).offset = offsetof(Vertex, texCoord);
    return attributeDescriptions;
}
