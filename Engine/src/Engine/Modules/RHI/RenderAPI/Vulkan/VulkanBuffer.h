#pragma once
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
