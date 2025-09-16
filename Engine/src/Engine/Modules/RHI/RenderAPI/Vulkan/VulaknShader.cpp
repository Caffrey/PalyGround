#include "VulaknShader.h"

void VulkanShader::InitShader(VulkanContext* Context, const std::vector<char>& Code)
{
    VkShaderModuleCreateInfo shaderModuleCreateInfo{};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = Code.size();
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(Code.data());

    vkCreateShaderModule(Context->Device,&shaderModuleCreateInfo,nullptr,&shaderModule);

    SetupShaderStage();
}

void VulkanVertexShader::SetupShaderStage()
{
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; 
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";
}

void VulkanPixelShader::SetupShaderStage()
{
    
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; 
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";

}



void VulkanMaterialShader::InitMaterialShader(VulkanContext* Context, const std::vector<char>& VertexCode,
    const std::vector<char>& PixelCode)
{
    VertexShader = new VulkanVertexShader();
    PixelShader = new VulkanPixelShader();

    VertexShader->InitShader(Context,VertexCode);
    PixelShader->InitShader(Context,PixelCode);
}

void VulkanMaterialShader::CreateShaderPipeline()
{
    VkPipelineShaderStageCreateInfo ShaderStageCreateInfo []= {VertexShader->shaderStageInfo,PixelShader
    ->shaderStageInfo};
    
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = ShaderStageCreateInfo;

    
    // pipelineInfo.pVertexInputState = &vertexInputInfo;
    // pipelineInfo.pInputAssemblyState = &inputAssembly;
    // pipelineInfo.pRasterizationState = &rasterizer;
    // pipelineInfo.pMultisampleState = &multisampling;
    // pipelineInfo.pDepthStencilState = nullptr; // Optional
    // pipelineInfo.pColorBlendState = &colorBlending;
    // pipelineInfo.pDynamicState = &dynamicState;
    // pipelineInfo.layout = pipelineLayout;

    

    // VkPipelineDynamicStateCreateInfo
    // VkPipelineVertexInputStateCreateInfo
    // VkPipelineInputAssemblyStateCreateInfo 
    // VkPipelineRasterizationStateCreateInfo
    // VkPipelineViewportStateCreateInfo
    // VkPipelineRasterizationStateCreateInfo
    // VkPipelineMultisampleStateCreateInfo
    // VkPipelineColorBlendAttachmentState
    // VkPipelineColorBlendStateCreateInfo
    // VkPipelineLayoutCreateInfo 
}
