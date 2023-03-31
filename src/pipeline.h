#pragma once

#include "vulkan_device.h"

struct PipelineConfigInfo
{

};

class Pipeline
{
public:
    Pipeline(VulkanDevice& device, const PipelineConfigInfo& configInfo);
    ~Pipeline();
    Pipeline(const Pipeline& other) = delete;
    Pipeline(Pipeline&& other) = delete;
    auto operator=(const Pipeline& other) -> Pipeline& = delete;
    auto operator=(Pipeline&& other) -> Pipeline& = delete;

    static auto defaultPipelineConfigInfo(uint32_t width, uint32_t height) -> PipelineConfigInfo;

private:
    auto createGraphicsPipeline(const PipelineConfigInfo& configInfo) -> void;
    auto createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) -> void;

    VulkanDevice& m_device;
    VkPipeline m_graphicsPipeline;
    VkShaderModule m_vertShaderModule;
    VkShaderModule m_fragShaderModule;
};
