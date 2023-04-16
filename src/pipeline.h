#pragma once

#include "vulkan_device.h"
#include <vector>

struct PipelineConfigInfo
{
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo& other) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo& other) = delete;

    VkPipelineViewportStateCreateInfo viewportInfo{};
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
    VkPipelineMultisampleStateCreateInfo multisampleInfo{};
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class Pipeline
{
public:
    Pipeline(VulkanDevice& device, const PipelineConfigInfo& configInfo, std::vector<char>& vertCode, std::vector<char>& fragCode);
    ~Pipeline();
    Pipeline(const Pipeline& other) = delete;
    Pipeline(Pipeline&& other) = delete;
    auto operator=(const Pipeline& other) -> Pipeline& = delete;
    auto operator=(Pipeline&& other) -> Pipeline& = delete;

    static auto defaultPipelineConfigInfo(PipelineConfigInfo& configInfo) -> void;
    auto bind(VkCommandBuffer commandBuffer) -> void;

private:
    auto createGraphicsPipeline(const PipelineConfigInfo& configInfo, std::vector<char>& vertCode, std::vector<char>& fragCode) -> void;
    auto createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) -> void;

    VulkanDevice& m_device;
    VkPipeline m_graphicsPipeline;
    VkShaderModule m_vertShaderModule;
    VkShaderModule m_fragShaderModule;
};
