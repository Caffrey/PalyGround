#pragma once
#include "Core/Math/vector3.h"
#include "Core/Math/vector4.h"
#include "Modules/RHI/RHIBuffer.h"
#include "vulkan/vulkan.h"
#include "vma/vk_mem_alloc.h"
class VulkanBuffer : public RHIBuffer
{
public:
    static VulkanBuffer* CreateBuffer(VmaAllocator Allactor, size_t allocSize, VkBufferCreateFlags usage, VmaMemoryUsage memory_usage);

public:
    VkBuffer Buffer;
    VmaAllocation Allocation;
    VmaAllocationInfo Info;
};


struct VertexFormat
{
public:
    VertexFormat(){};
    VertexFormat(std::vector<Vector3> InPosition,
        std::vector<Vector3> InNomral,
        std::vector<Vector3> InColor,
        std::vector<Vector4> InUV1,
        std::vector<Vector4> InUV2,     
        std::vector<Vector4> InUV3,
        std::vector<Vector4> InUV4
        );
    VertexFormat(int VSize,int VertexCount,std::vector<float> Data, std::vector<int>& Offsets);

    
protected:
    VkVertexInputAttributeDescription CreateAttributeDesc(int offset, int location, bool IsVector4);

    void PushVec3(std::vector<Vector3> Source);
    void PushVec4(std::vector<Vector4> Source);
    
public:
    int VertexSide;
    int VertexCount;
    std::vector<int>VertexOffset;
    std::vector<float> VertexData;

    VkVertexInputBindingDescription BindingDesc;
    std::vector<VkVertexInputAttributeDescription> AttributeDesc;
    
};

class VulkanVertexBuffer : public VulkanBuffer
{
public:
    VulkanVertexBuffer();
    ~VulkanVertexBuffer(){};
    static VulkanVertexBuffer* CreateVertexBuffer(VmaAllocator Allactor,int VertexSide,int VertexOffset[],int VertexOffsetCount, void*VertexData, int VertexCount);
};


