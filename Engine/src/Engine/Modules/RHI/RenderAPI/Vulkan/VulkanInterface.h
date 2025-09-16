#pragma once
#include "VkBootstrap.h"
#include "XSDLWindows.h"
#include "Modules/RHI/RHIInterface.h"
#include "vk_mem_alloc.h"
#include "TestDrawVulkan.h"

struct VKFrameData
{
    VkCommandPool CommandPool;
    VkCommandBuffer CommandBuffer;
    VkFence RenderFence;
    VkSemaphore SwapChainSemaphore,RenderSemaphore;
};
constexpr unsigned int FRAME_OVERLAP = 2;

class VulkanRHIInterface : public RHIInterface
{
public:
    void Init(XWindows* Window) override;
    void BeginRenderFrame() override;
    void EndRenderFrame() override;
    void TestDrawLoop() override;
    
private:
    void InitVulkanInterface();

    void InitMmeoryAllocator();
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

    VulkanContext RHIContext;

    TestDrawVulkan DrawVulkan;;
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
    VmaAllocator vmaAllocator;
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
