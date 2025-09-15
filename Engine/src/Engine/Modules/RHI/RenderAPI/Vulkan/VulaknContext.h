#pragma once

#include "VkBootstrap.h"
#include "XSDLWindows.h"
#include "Modules/RHI/RHIContext.h"
#include "Modules/RHI/RHIInterface.h"
#include "vma/vk_mem_alloc.h"

class VulaknContext : public RHIContext
{
public:
    vkb::Instance vkbInstance;
    VkInstance Instance;
    VkDebugUtilsMessengerEXT DebugMessage;
    VkPhysicalDevice PhysicalDevice;
    VkDevice Device;
    VkSurfaceKHR Surface;
    
};
