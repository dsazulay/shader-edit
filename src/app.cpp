#include "app.h"
#include "GLFW/glfw3.h"
#include "resource_manager.h"
#include "vulkan/vulkan_core.h"

#include <cstddef>
#include <stdexcept>
#include <array>

struct PushConstantData
{
    float time;
};

App::App()
{
    loadModel();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
}

App::~App()
{
    vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

auto App::run() -> void
{
    while(!m_window.shouldClose())
    {
        m_window.pollEvents();
        if (m_window.recompileShader)
        {
            m_window.recompileShader = false;
            
            vkDeviceWaitIdle(m_device.device());
            createPipeline();
        }
        drawFrame();
    }

    vkDeviceWaitIdle(m_device.device());
}

auto App::loadModel() -> void
{
    std::vector<Model::Vertex> vertices
    {
        {{-1.0, -1.0}, {0.0, 1.0}},
        {{1.0, 1.0}, {1.0, 0.0}},
        {{-1.0, 1.0}, {0.0, 0.0}},
        {{-1.0, -1.0}, {0.0, 1.0}},
        {{1.0, -1.0}, {1.0, 1.0}},
        {{1.0, 1.0}, {1.0, 0.0}},
    };

    m_model = std::make_unique<Model>(m_device, vertices);
}

auto App::createPipelineLayout() -> void
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 1; // Optionala
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // Optional

    if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

auto App::createPipeline() -> void
{
    //Shader* shader = ResourceManager::loadShader("../resources/unlit.vert.spv", "../resources/unlit.frag.spv", "Unlit", true);
    Shader* shader = ResourceManager::loadShader("../resources/unlit.vert", "../resources/unlit.frag", "Unlit", false);
    if (shader == nullptr)
        return;
    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = m_swapChain->getRenderPass();
    pipelineConfig.pipelineLayout = m_pipelineLayout;
    m_pipeline = std::make_unique<Pipeline>(m_device, pipelineConfig, shader->m_vertCode, shader->m_fragCode);
}

auto App::createCommandBuffers() -> void
{
    m_commandBuffers.resize(m_swapChain->imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_device.getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    if (vkAllocateCommandBuffers(m_device.device(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

auto App::freeCommandBuffers() -> void
{
    vkFreeCommandBuffers(m_device.device(), m_device.getCommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
    m_commandBuffers.clear();
}

auto App::drawFrame() -> void
{
    uint32_t imageIndex;
    auto result = m_swapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    recordCommandBuffer(imageIndex);
    result = m_swapChain->submitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window.wasWindowResized())
    {
        m_window.resetWindowResizedFlag();
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }
}

auto App::recreateSwapChain() -> void
{
    auto extent = m_window.getExtent();
    while (extent.width == 0 || extent.height == 0)
    {
        extent = m_window.getExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_device.device());
    if (m_swapChain == nullptr)
        m_swapChain = std::make_unique<SwapChain>(m_device, extent);
    else
    {
        m_swapChain = std::make_unique<SwapChain>(m_device, extent, std::move(m_swapChain));
        if (m_swapChain->imageCount() != m_commandBuffers.size())
        {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }
    createPipeline();
}

auto App::recordCommandBuffer(int imageIndex) -> void
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(m_commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_swapChain->getRenderPass();
    renderPassInfo.framebuffer = m_swapChain->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};

    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_swapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(m_swapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, m_swapChain->getSwapChainExtent()};
    vkCmdSetViewport(m_commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(m_commandBuffers[imageIndex], 0, 1, &scissor);

    m_pipeline->bind(m_commandBuffers[imageIndex]);

    m_model->bind(m_commandBuffers[imageIndex]);

    PushConstantData pushConstantData{};
    pushConstantData.time = static_cast<float>(glfwGetTime());
    vkCmdPushConstants(m_commandBuffers[imageIndex], m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
            0, sizeof(PushConstantData), &pushConstantData);

    m_model->draw(m_commandBuffers[imageIndex]);

    vkCmdEndRenderPass(m_commandBuffers[imageIndex]);

    if (vkEndCommandBuffer(m_commandBuffers[imageIndex]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }

}
