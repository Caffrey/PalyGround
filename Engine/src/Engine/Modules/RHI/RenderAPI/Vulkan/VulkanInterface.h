#pragma once
#include "VkBootstrap.h"
#include "XSDLWindows.h"
#include "Modules/RHI/RHIInterface.h"


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
    void Init(XWindows* Window) override;
    void BeginRenderFrame() override;
    void EndRenderFrame() override;
        
    
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

    XSDLWindows * SDLWindow;

//Interface
    public:
    //Buffer
    virtual RHIBuffer* CreateVertexBuffer() {return nullptr;};
    virtual RHITexture* CreateTexture() {return nullptr;};
    virtual RHITexture* CreateRenderTarget() {return nullptr;};

    //Shader
    virtual RHIShader* CreateVertexShader() {return nullptr;};
    virtual RHIShader* CreatePixelShader() {return nullptr;};
    virtual RHIShader* CreateComputeShader() {return nullptr;};

    //Command
    virtual RHICommandBuffer* GetCommandBuffer() {return nullptr;};
    virtual void ExecuteCommandBuffer(RHICommandBuffer*){};
    virtual void SubmitCommands(){};


};
