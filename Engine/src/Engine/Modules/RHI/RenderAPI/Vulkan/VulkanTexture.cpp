#include "VulkanTexture.h"

VulkanTexture* VulkanTexture::CreateImage(VmaAllocator Allactor, size_t allocSize, VkBufferCreateFlags usage,
    VmaMemoryUsage memory_usage)
{
    VkImageCreateInfo ImageInfo{.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    ImageInfo.pNext = nullptr;
    ImageInfo.flags = 0;
    ImageInfo.imageType = VK_IMAGE_TYPE_2D;
    ImageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    ImageInfo.extent = VkExtent3D{1024,1024,1};
    ImageInfo.mipLevels = 1;
    ImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    ImageInfo.tiling = VK_IMAGE_TILING_LINEAR;
    ImageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
    ImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VmaAllocationCreateInfo VmaAllocationCreateInfo = {};
    VmaAllocationCreateInfo.usage = memory_usage;
    VmaAllocationCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

    VulkanTexture* TextureBuffer = new VulkanTexture();
    
    vmaCreateImage(Allactor, &ImageInfo, &VmaAllocationCreateInfo, &TextureBuffer->Image,&TextureBuffer->Allocation,nullptr);
}
