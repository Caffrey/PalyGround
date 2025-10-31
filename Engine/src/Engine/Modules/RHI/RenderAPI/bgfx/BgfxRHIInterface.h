#pragma once
#include "BgfxRHIBuffer.h"
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
    virtual RHIBuffer* CreateVertexBuffer() override{return nullptr;};
    virtual RHIBuffer* CreateVertexBuffer2(){return new BgfxRHIVertexBuffer();};
    virtual RHIBuffer* CreateIndexBuffer(){return new BgfxRHIIndexBuffer();};
    virtual RHITexture* CreateTexture2D() override{return new RHITexture2D();};
    
    
    RHITexture* CreateTexture() override{return nullptr;};
    RHITexture* CreateRenderTarget() override{return nullptr;};

    //Shader
    RHIShader* CreateVertexShader()override {return nullptr;};
    RHIShader* CreatePixelShader() override{return nullptr;};
    RHIShader* CreateComputeShader() override{return nullptr;};

    //Command
    virtual RHICommandBuffer* GetCommandBuffer()override {return nullptr;};
    virtual void ExecuteCommandBuffer(RHICommandBuffer*) {};
    virtual void SubmitCommands() {};
};
