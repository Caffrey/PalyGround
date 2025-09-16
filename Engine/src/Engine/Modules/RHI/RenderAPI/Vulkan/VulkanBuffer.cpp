#include "VulkanBuffer.h"
#include "vulkan/vulkan.h"
#include "vma/vk_mem_alloc.h"



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
