#pragma once
#include "Modules/RHI/RHIInterface.h"
#include "Modules/RHI/RenderAPI/Vulkan/XSDLWindows.h"

class BgfxRHIInterface : public RHIInterface
{
public:
    void Init(XWindows* Window) override;
    void BeginRenderFrame() override;
    void EndRenderFrame() override;

    XSDLWindows * SDLWindow;
    virtual void TestDrawLoop(){};

    
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
    virtual void ExecuteCommandBuffer(RHICommandBuffer*) {};
    virtual void SubmitCommands() {};
};
