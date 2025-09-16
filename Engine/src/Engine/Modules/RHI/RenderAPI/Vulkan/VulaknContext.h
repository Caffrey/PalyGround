#pragma once

#include "VkBootstrap.h"
#include "XSDLWindows.h"
#include "Modules/RHI/RHIContext.h"
#include "Modules/RHI/RHIInterface.h"
#include "vma/vk_mem_alloc.h"

class VulkanContext : public RHIContext
{
public:
    VkInstance Instance;
    VkDebugUtilsMessengerEXT DebugMessage;
    VkPhysicalDevice PhysicalDevice;
    VkDevice Device;
    VkSurfaceKHR Surface;
    VmaAllocator Allocator;
    
};
