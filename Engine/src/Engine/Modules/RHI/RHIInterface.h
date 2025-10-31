#pragma once
#include "RHIBuffer.h"
#include "RHICommandBuffer.h"
#include "RHIShader.h"
#include "RHITexture.h"
class XWindows;


class RHIInterface
{
public:
    virtual void Init(XWindows* Window);

    XWindows* Window;

    virtual void BeginRenderFrame() = 0;
    virtual void EndRenderFrame() = 0;
    virtual void TestDrawLoop(){};

    
public:
    //Buffer
    virtual RHIBuffer* CreateVertexBuffer() = 0;
    virtual RHIBuffer* CreateVertexBuffer2(RHIVertexLayoutDesc){return nullptr;};
    virtual RHITexture* CreateTexture() = 0;
    virtual RHITexture* CreateTexture2D() = 0;
    virtual RHITexture* CreateRenderTarget() = 0;

    //Shader
    virtual RHIShader* CreateVertexShader() = 0;
    virtual RHIShader* CreatePixelShader() = 0;
    virtual RHIShader* CreateComputeShader() = 0;

    //Command
    virtual RHICommandBuffer* GetCommandBuffer() = 0;
    virtual void ExecuteCommandBuffer(RHICommandBuffer*) = 0;
    virtual void SubmitCommands() = 0;

    

};
