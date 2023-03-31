#include "pipeline.h"

Pipeline::Pipeline(VulkanDevice& device, const PipelineConfigInfo& configInfo) : m_device(device)
{
    createGraphicsPipeline(configInfo);
}

Pipeline::~Pipeline()
{

}

auto Pipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) -> PipelineConfigInfo
{
    PipelineConfigInfo configInfo;
    return configInfo;
}

auto Pipeline::createGraphicsPipeline(const PipelineConfigInfo& configInfo) -> void
{

}

auto Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) -> void
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(m_device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader modele");
    }
}
