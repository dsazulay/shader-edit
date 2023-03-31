#pragma once

#include "window.h"
#include "vulkan_device.h"
#include "pipeline.h"

class App
{
public:
    auto run() -> void;

private:
    Window m_window{WIDTH, HEIGHT, "Shader Edit"};
    VulkanDevice m_device{m_window};
    Pipeline m_pipeline{m_device, Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};

    static constexpr int WIDTH = 960;
    static constexpr int HEIGHT = 540;
};
