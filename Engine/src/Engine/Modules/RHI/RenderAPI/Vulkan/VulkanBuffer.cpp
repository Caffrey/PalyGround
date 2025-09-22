#include "VulkanBuffer.h"
#include "vulkan/vulkan.h"
#include "vma/vk_mem_alloc.h"
#include "VulkanUtils.h"



VulkanBuffer* VulkanBuffer::CreateBuffer(VmaAllocator Allactor, size_t allocSize, VkBufferCreateFlags usage,
    VmaMemoryUsage memory_usage)
{
    VkBufferCreateInfo BufferCreateInfo = {.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    BufferCreateInfo.pNext = nullptr;
    BufferCreateInfo.size = allocSize;

    BufferCreateInfo.usage = usage;
    

    VmaAllocationCreateInfo VmaAllocationCreateInfo = {};
    VmaAllocationCreateInfo.usage = memory_usage;
    VmaAllocationCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

    VulkanBuffer* buffer = new VulkanBuffer();;
    // vmaCreateBuffer(Allactor,&BufferCreateInfo,&VmaAllocationCreateInfo,&buffer.Buffer,&buffer.Allocation,nullptr);
    return buffer;
}

VertexFormat::VertexFormat(std::vector<Vector3> InPosition, std::vector<Vector3> InNomral, std::vector<Vector3> InColor,
    std::vector<Vector4> InUV1, std::vector<Vector4> InUV2, std::vector<Vector4> InUV3, std::vector<Vector4> InUV4)
{

    VertexSide = 0;
    int Vec3Size = sizeof(Vector3);
    int Vec4Size = sizeof(Vector4);
    
    if(!InPosition.empty())
    {
        VertexSide += Vec3Size;
        VertexOffset.push_back(0);
        PushVec3(InPosition);
        
    }
    if(!InNomral.empty())
    {
        VertexSide += Vec3Size;
        VertexOffset.push_back(Vec3Size);
        PushVec3(InNomral);
    }
    if(!InColor.empty())
    {
        VertexSide += Vec3Size;
        VertexOffset.push_back(Vec3Size);
        PushVec3(InColor);
    }
    if(!InUV1.empty())
    {
        VertexSide += Vec4Size;
        VertexOffset.push_back(Vec4Size);
        PushVec4(InUV1);
    }
    if(!InUV2.empty())
    {
        VertexSide += Vec4Size;
        VertexOffset.push_back(Vec4Size);
        PushVec4(InUV3);
    }
    if(!InUV3.empty())
    {
        VertexSide += Vec4Size;
        VertexOffset.push_back(Vec4Size);
        PushVec4(InUV3);
    }
    if(!InUV4.empty())
    {
        VertexSide += Vec4Size;
        VertexOffset.push_back(Vec4Size);
        PushVec4(InUV4);
    }

    VertexCount = InPosition.size();

    BindingDesc.binding = 0;
    BindingDesc.stride = VertexSide;
    BindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    for(int i = 0; i < VertexOffset.size(); i++)
    {
        int Offset = VertexOffset[i];
        AttributeDesc.push_back(CreateAttributeDesc(Offset,i,Offset == Vec4Size));
    }
}

VertexFormat::VertexFormat(int VSize,int InVertexCount,std::vector<float> Data, std::vector<int>& Offsets)
{
    VertexSide = VSize;
    VertexCount = InVertexCount;
    for(auto offset : Offsets)
    {
        VertexOffset.push_back(offset);
    }
    for(auto vertex : Data)
    {
        VertexData.push_back(vertex);
    }

    BindingDesc.binding = 0;
    BindingDesc.stride = VertexSide;
    BindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    int Vec4Size = sizeof(Vector4);

    for(int i = 0; i < VertexOffset.size(); i++)
    {
        int Offset = VertexOffset[i];
        AttributeDesc.push_back(CreateAttributeDesc(Offset,i,Offset == Vec4Size));
    }
}

VkVertexInputAttributeDescription VertexFormat::CreateAttributeDesc(int offset, int location, bool IsVector4)
{
    VkVertexInputAttributeDescription positionDescription = {};
    positionDescription.binding = 0;
    positionDescription.location = location;
    if(IsVector4)
    {
        positionDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    }
    else
    {
        positionDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
    }
    positionDescription.offset = offset;

    return positionDescription;
}

void VertexFormat::PushVec3(std::vector<Vector3> Source)
{
    for(auto vec : Source)
    {
        VertexData.push_back(vec.x);
        VertexData.push_back(vec.y);
        VertexData.push_back(vec.z);
    }
}

void VertexFormat::PushVec4(std::vector<Vector4> Source)
{
    for(auto vec : Source)
    {
        VertexData.push_back(vec.x);
        VertexData.push_back(vec.y);
        VertexData.push_back(vec.z);
        VertexData.push_back(vec.w);
    }
}

VulkanVertexBuffer::VulkanVertexBuffer()
{
}

VulkanVertexBuffer* VulkanVertexBuffer::CreateVertexBuffer(VmaAllocator Allactor,int VertexSide, int VertexOffset[], int VertexOffsetCount,
    void* VertexData,int VertexCount)
{
    VulkanVertexBuffer* Buffer = new  VulkanVertexBuffer();
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    //this is the total size, in bytes, of the buffer we are allocating
    bufferInfo.size = VertexCount * VertexSide;
    //this buffer is going to be used as a Vertex Buffer
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    VmaAllocationCreateInfo vmaallocInfo = {};
    vmaallocInfo.usage =  VMA_MEMORY_USAGE_CPU_TO_GPU;

    vmaCreateBuffer(Allactor, &bufferInfo, &vmaallocInfo,
        &Buffer->Buffer,
        &Buffer->Allocation,
        nullptr);

    void* Data;
    vmaMapMemory(Allactor,Buffer->Allocation,&Data);
    memcpy(Data,VertexData,VertexCount * VertexSide);
    vmaUnmapMemory(Allactor,Buffer->Allocation);

    return Buffer;
}
