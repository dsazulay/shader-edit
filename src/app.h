#pragma once

#include "window.h"
#include "vulkan_device.h"
#include "pipeline.h"
#include "swap_chain.h"

#include <memory>
#include <vector>

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
    auto createPipelineLayout() -> void;
    auto createPipeline() -> void;
    auto createCommandBuffers() -> void;
    auto drawFrame() -> void;

    Window m_window{WIDTH, HEIGHT, "Shader Edit"};
    VulkanDevice m_device{m_window};
    SwapChain m_swapChain{m_device, m_window.getExtent()};
    std::unique_ptr<Pipeline> m_pipeline;
    VkPipelineLayout m_pipelineLayout;
    std::vector<VkCommandBuffer> m_commandBuffers;

    static constexpr int WIDTH = 960;
    static constexpr int HEIGHT = 540;
};
