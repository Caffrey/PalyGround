#pragma once

#include "Modules/Windows/XSDLWindows.h"
#include <vulkan/vulkan.h>
#include "VkBootstrap.h"

class RHIInterface
{
public:
    virtual void Init(XSDLWindows* Window);


    XSDLWindows* SDLWindow;

    virtual void BeginDraw(){};
    virtual void EndDraw(){};
};

struct VKFrameData
{
    VkCommandPool CommandPool;
    VkCommandBuffer CommandBuffer;
    VkFence RenderFence;
    VkSemaphore SwapChainSemaphore,RenderSemaphore;
};
constexpr unsigned int FRAME_OVERLAP = 2;

class VulkanRHIInterface : RHIInterface
{
public:
    void Init(XSDLWindows* Window) override;
    void BeginDraw() override;
    void EndDraw() override;
        
    
private:
    void InitVulkanInterface();

    //Init Instance
    //Init Surface
    void InitInstance();
    //InitPhysical and Virtual Device
    void InitDevice();
    void InitSwapChain();
    void InitCommand();

    void DestoryIntercace();

private:
    vkb::Instance vkbInstance;
    VkInstance Instance;
    VkDebugUtilsMessengerEXT DebugMessage;
    VkPhysicalDevice PhysicalDevice;
    VkDevice Device;
    VkSurfaceKHR Surface;


    VkSwapchainKHR SwpahChain;
    VkFormat ScreenRenderTargetFormat;
    std::vector<VkImage> SwapChainImages;
    std::vector<VkImageView> SwapChainImageViews;
    VkExtent2D SwapChainExtent;


    //Command
    VKFrameData Frames[FRAME_OVERLAP];
    int FrameNumber;
    VKFrameData& GetCurrentFrame() { return Frames[FrameNumber % FRAME_OVERLAP]; };

    VkQueue GraphicQueue;
    uint32_t GraphicQueueFamily;

    
    
};