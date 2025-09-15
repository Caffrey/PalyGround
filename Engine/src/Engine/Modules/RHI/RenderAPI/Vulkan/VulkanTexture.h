#pragma once
#include "Modules/RHI/RHITexture.h"
#include "vulkan/vulkan.h"
#include "vma/vk_mem_alloc.h"    
class VulkanTexture : public RHITexture
{
public:
    static VulkanTexture* CreateImage(VmaAllocator Allactor, size_t allocSize, VkBufferCreateFlags usage, VmaMemoryUsage memory_usage);

    VkImage Image;
    VkImageView ImageView;
    VmaAllocation Allocation;
};
 