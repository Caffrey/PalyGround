#pragma once
#include <vector>
#include "vulkan/vulkan.h"
#include "Core/Math/vector3.h"
#include "VulaknContext.h"
class TestDrawVulkan
{
public:
    struct VertexStruct
    {
    public:
        Vector3 pos;
        Vector3 color;
        Vector3 normal;
    };
    
    void Draw(VkCommandBuffer& Buffer);


    void Init(VulkanContext& Context);

    void InitMesh(VulkanContext& Context);
    void InitPipeline(VulkanContext& Context);
    void InitInputDesc();
    void InitShader(VulkanContext& Context);
    void createRenderPass(VulkanContext& Context);
    void load_shader_module(VulkanContext& Context, const char*FilePath, VkShaderModule* OutModule);
    
    std::vector<VertexStruct> Vertex;
    VmaAllocation VertexAllocation;

    VkPipelineLayout PipelineLayout;
    VkPipeline GraphicPipeline;
    std::vector<VkVertexInputBindingDescription> InputBindings;
    std::vector<VkVertexInputAttributeDescription> InputAttributes;
    VkPipelineVertexInputStateCreateFlags  flag = 0;


    VkVertexInputAttributeDescription CreateAttributeDesc(int offset, int location);
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
VkRenderPass renderPass;
    VkShaderModule VertexShader;
    VkShaderModule PixelShader;
};
