#pragma once
#include <vector>
#include "vulkan/vulkan.h"
#include "Core/Math/vector3.h"
#include "VulaknContext.h"
class TestDrawVulkan
{
public:
    void Draw(VkCommandBuffer& Buffer);


    void Init(VulkanContext& Context);

    void InitMesh(VulkanContext& Context);
    void InitPipeline(VulkanContext& Context);
    void InitInputDesc();
    void InitShader(VulkanContext& Context);

    void load_shader_module(VulkanContext& Context, const char*FilePath, VkShaderModule* OutModule);
    
    std::vector<Vector3> Vertex;
    std::vector<Vector3> Colors;
    std::vector<Vector3> Buffers;
    VmaAllocation VertexAllocation;

    VkPipelineLayout PipelineLayout;
    VkPipeline GraphicPipeline;
    std::vector<VkVertexInputBindingDescription> InputBindings;
    std::vector<VkVertexInputAttributeDescription> InputAttributes;
    VkPipelineVertexInputStateCreateFlags  flag = 0;


    VkVertexInputAttributeDescription CreateAttributeDesc(int offset, int location);
    VkBuffer vertexBuffer;;


    VkShaderModule VertexShader;
    VkShaderModule PixelShader;
};
