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

class VulkanShaderModule 
{

    
    VkShaderModule shaderModule;  
};

class VulkanComputeShader : public RHIShader
{
    VkShaderModule shaderModule;
    
};

class VulkanMaterialShader : public RHIShader
{
public:

    static void CreateShader(VulaknContext*Context, const std::vector<char>& code);
    void UseShader();

    VkShaderModule shaderModule;
    VulaknShaderBlendMode BlendMode;
    VulkanShaderStencilMode Mode;

    VkPipeline ShaderPipeline;
    
    
    //Descriptor Sets
    //eDescriptorPool


    //Shader Module -> Shader Code
    //Shader Stage -> Application Of Module -> Vertex Module -> Vertex Stage
    //Pipeline - > Shdaer Stage Composition
    //Descriptor -> Resources That Pipeline Use; 
};
