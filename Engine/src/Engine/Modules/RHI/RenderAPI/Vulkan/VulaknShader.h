#pragma once
#include <vector>
#include "Modules/RHI/RenderAPI/Vulkan/VulaknContext.h"
#include "Modules/RHI/RHIShader.h"
#include "vulkan/vulkan.h"
#include "vma/vk_mem_alloc.h"

struct VulaknShaderBlendMode
{
public:
    int SrcColorBlend;
    int DstColorBlend;
    int SrcAlphaBlend;
    int DstAlphaBlend;
    int BlendFun;
};

struct VulkanShaderStencilMode
{
public:
    int Ref;
    int ReadMask;
    int WriteMask;
    int Comp;
    int Pass;
    int Fail;
    int ZFail;
    int CompBack;
    int PassBack;
    int FailBack;
    int ZFailBack;
    int CompFront;
    int PassFront;
    int FailFront;
    int ZFailFront;
};


class VulkanShader : public RHIObject
{
public:
    void InitShader(VulkanContext*Context, const std::vector<char>& Code);
    VkPipelineShaderStageCreateInfo shaderStageInfo;
    
protected:
    virtual void SetupShaderStage() = 0;
    
    VkShaderModule shaderModule;
};

class VulkanVertexShader : public VulkanShader
{
protected:
    void SetupShaderStage() override;
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
};

class VulkanPixelShader : public VulkanShader
{
protected:
    void SetupShaderStage() override;

    VkPipelineRasterizationStateCreateInfo rasterStateInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo;
    VkPipelineColorBlendStateCreateInfo colorBlendStateInfo;
};

class VulkanComputeShader : public RHIShader
{
    VkShaderModule shaderModule;
};

class VulkanMaterialShader : public RHIShader
{
public:

    void InitMaterialShader(VulkanContext*Context, const std::vector<char>& VertexCode, const std::vector<char>& PixelCode);
protected:
    void CreateShaderPipeline();

    VulkanVertexShader* VertexShader;
    VulkanPixelShader* PixelShader;
    
    VulaknShaderBlendMode BlendMode;
    VulkanShaderStencilMode Mode;

    VkPipeline ShaderPipeline;
    
    
    //Descriptor Sets
    //eDescriptorPool


    //Shader Module -> Shader Code
    //Shader Stage -> Application Of Module -> Vertex Module -> Vertex Stage
    //Shader Module -> Connect Shader Stage
    //Pipeline - > Shdaer Stage Composition
    //Descriptor -> Resources That Pipeline Use; 
};
