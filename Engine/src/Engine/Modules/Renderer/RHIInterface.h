#pragma once

#include "VkBootstrap.h"
class XSDLWindows;
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

class VulkanUtils
{
public:
    static void transition_image(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);
    static VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspectMask);
    static VkSemaphoreSubmitInfo semaphore_submit_info(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore)
    {
        VkSemaphoreSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        submitInfo.semaphore = semaphore;
        submitInfo.stageMask = stageMask;
        submitInfo.deviceIndex = 0;
        submitInfo.value = 1;

        return submitInfo;
    }

    static VkCommandBufferSubmitInfo command_buffer_submit_info(VkCommandBuffer cmd)
    {
        VkCommandBufferSubmitInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
        info.pNext = nullptr;
        info.commandBuffer = cmd;
        info.deviceMask = 0;

        return info;
    }

    static VkSubmitInfo2 submit_info(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo,
        VkSemaphoreSubmitInfo* waitSemaphoreInfo)
    {
        VkSubmitInfo2 info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
        info.pNext = nullptr;

        info.waitSemaphoreInfoCount = waitSemaphoreInfo == nullptr ? 0 : 1;
        info.pWaitSemaphoreInfos = waitSemaphoreInfo;

        info.signalSemaphoreInfoCount = signalSemaphoreInfo == nullptr ? 0 : 1;
        info.pSignalSemaphoreInfos = signalSemaphoreInfo;

        info.commandBufferInfoCount = 1;
        info.pCommandBufferInfos = cmd;

        return info;
    }
    
};

class VulkanRHIInterface : public RHIInterface
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


    VkSwapchainKHR SwaphChain;
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
    uint32_t SwapchainImageIndex;

    
};