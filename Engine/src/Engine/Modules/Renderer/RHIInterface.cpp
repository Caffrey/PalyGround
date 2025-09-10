#include "RHIInterface.h"
#include <SDL3/SDL_vulkan.h>



void RHIInterface::Init(XSDLWindows* Window)
{
    this->SDLWindow = Window;
}

void VulkanRHIInterface::Init(XSDLWindows* Window)
{
    RHIInterface::Init(Window);

    InitVulkanInterface();

    
}

void VulkanRHIInterface::BeginDraw()
{
    vkWaitForFences(this->Device,1,&GetCurrentFrame().RenderFence,true,1000000);
    vkResetFences(this->Device,1,&GetCurrentFrame().RenderFence);

    uint32_t swapchainImageIndex;
    vkAcquireNextImageKHR(this->Device, this->SwpahChain, 1000000000, GetCurrentFrame().SwapChainSemaphore, nullptr, &swapchainImageIndex);

    VkCommandBuffer cmd = GetCurrentFrame().CommandBuffer;
    vkResetCommandBuffer(cmd,0);
    
}

void VulkanRHIInterface::EndDraw()
{
    
}

void VulkanRHIInterface::InitVulkanInterface()
{

    InitInstance();
    InitDevice();
    InitSwapChain();
    InitCommand();
}

void VulkanRHIInterface::InitInstance()
{
    vkb::InstanceBuilder vkBuilder;
    auto inst_ret = vkBuilder.set_app_name("Vulkan Application")
   .request_validation_layers(false)
   .use_default_debug_messenger()
   .require_api_version(1,3,0)
   .build();
    
    this->vkbInstance = inst_ret.value();

    this->Instance = vkbInstance.instance;
    this->DebugMessage = vkbInstance.debug_messenger;
    
    SDL_Vulkan_CreateSurface(this->SDLWindow->GetWindow(),this->Instance,nullptr,&this->Surface);
}

void VulkanRHIInterface::InitDevice()
{

    //setup feature
    VkPhysicalDeviceVulkan13Features features13{};
    features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    features13.dynamicRendering = true;
    features13.synchronization2 = true;

    //vulkan 1.2 features
    VkPhysicalDeviceVulkan12Features features12{  };
    features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    features12.bufferDeviceAddress = true;
    features12.descriptorIndexing = true;

    
    vkb::PhysicalDeviceSelector selector{vkbInstance};
    vkb::PhysicalDevice physical_device = selector
    .set_minimum_version(1,3)
    .set_required_features_13(features13)
    .set_required_features_12(features12)
    .set_surface(this->Surface)
    .select().value();

    vkb::DeviceBuilder device_builder{physical_device};
    vkb::Device vkbDevice = device_builder.build().value();

    this->Device = vkbDevice.device;
    this->PhysicalDevice = physical_device.physical_device;

    this->GraphicQueue =  vkbDevice.get_queue(vkb::QueueType::graphics).value();
    this->GraphicQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

}

void VulkanRHIInterface::InitSwapChain()
{
    vkb::SwapchainBuilder builder(PhysicalDevice,Device,Surface);
    ScreenRenderTargetFormat = VK_FORMAT_R8G8B8A8_UNORM;

    vkb::Swapchain vkbSwapChain = builder
    .set_desired_format(VkSurfaceFormatKHR{.format = ScreenRenderTargetFormat, .colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR})
    .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
    .set_desired_extent(this->SDLWindow->Width,this->SDLWindow->Height)
    .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
    .build().value();

    SwapChainExtent = vkbSwapChain.extent;
    this->SwpahChain = vkbSwapChain.swapchain;
    this->SwapChainImages = vkbSwapChain.get_images().value();
    this->SwapChainImageViews = vkbSwapChain.get_image_views().value();
    
    
}

void VulkanRHIInterface::InitCommand()
{
    VkCommandPoolCreateInfo commandPoolInfo =  {};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.pNext = nullptr;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = this->GraphicQueueFamily;;

    for (int i = 0; i < FRAME_OVERLAP; i++) {

        
        vkCreateCommandPool(this->Device, &commandPoolInfo, nullptr, &this->Frames[i].CommandPool);

        // allocate the default command buffer that we will use for rendering
        VkCommandBufferAllocateInfo cmdAllocInfo = {};
        cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdAllocInfo.pNext = nullptr;
        cmdAllocInfo.commandPool = this->Frames[i].CommandPool;
        cmdAllocInfo.commandBufferCount = 1;
        cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

        vkAllocateCommandBuffers(this->Device, &cmdAllocInfo, &this->Frames[i].CommandBuffer);


        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        
        VkSemaphoreCreateInfo semaphoreCreateInfo = {};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext = nullptr;
        semaphoreCreateInfo.flags = 0;
        
        //singal
        vkCreateFence(this->Device, &fenceCreateInfo, nullptr, &this->Frames[i].RenderFence);
        vkCreateSemaphore(this->Device, &semaphoreCreateInfo, nullptr, &this->Frames[i].SwapChainSemaphore);
        vkCreateSemaphore(this->Device, &semaphoreCreateInfo, nullptr, &this->Frames[i].RenderSemaphore);
    }
    
}

void VulkanRHIInterface::DestoryIntercace()
{
    for (int i = 0; i < FRAME_OVERLAP; i++) {
        vkDestroyCommandPool(this->Device, this->Frames[i].CommandPool, nullptr);
    }
    
    vkDestroySwapchainKHR(Device,this->SwpahChain,nullptr);


    
    for(int i = 0; i < this->SwapChainImageViews.size(); i++)
    {
        vkDestroyImageView(Device,this->SwapChainImageViews[i],nullptr);
    }

    vkDestroySurfaceKHR(this->Instance,this->Surface,nullptr);
    vkDestroyDevice(this->Device,nullptr);

    vkb::destroy_debug_utils_messenger(this->Instance,this->DebugMessage);
    vkDestroyInstance(this->Instance,nullptr);
}
