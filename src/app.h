#pragma once

#include "window.h"
#include "vulkan_device.h"
#include "pipeline.h"
#include "swap_chain.h"
#include "model.h"

#include <memory>
#include <vector>
#include <string_view>

class App
{
public:
    App();
    ~App();
    App(const App& other) = delete;
    App(App&& other) = delete;
    auto operator=(const App& other) -> App& = delete;
    auto operator=(App&& other) -> App& = delete;
    auto run() -> void;

private:
    auto loadModel() -> void;
    auto createPipelineLayout() -> void;
    auto createPipeline() -> void;
    auto createCommandBuffers() -> void;
    auto freeCommandBuffers() -> void;
    auto drawFrame() -> void;
    auto recreateSwapChain() -> void;
    auto recordCommandBuffer(int imageIndex) -> void;

    auto createImageTexture(std::string_view path) -> void;
    auto createTextureImageView() -> void;
    auto createTextureSampler() -> void;
    auto createDescriptorSetLayout() -> void;
    auto createDescriptorSets() -> void;
    auto createDescriptorPool() -> void;

    Window m_window{WIDTH, HEIGHT, "Shader Edit"};
    VulkanDevice m_device{m_window};
    std::unique_ptr<SwapChain> m_swapChain;
    std::unique_ptr<Pipeline> m_pipeline;
    VkPipelineLayout m_pipelineLayout;
    std::vector<VkCommandBuffer> m_commandBuffers;
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_imageView;
    VkSampler m_sampler;
    VkDescriptorSetLayout m_descriptorSetLayout;
    std::vector<VkDescriptorSet> m_descriptorSets;
    VkDescriptorPool m_descriptorPool;

    std::unique_ptr<Model> m_model;

    static constexpr int WIDTH = 960;
    static constexpr int HEIGHT = 540;
};
