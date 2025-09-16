#pragma once

class RHICommandBuffer
{
public:
    

    virtual void SetRenderTarget() = 0;
    virtual void SetupViewPort() = 0;


    virtual void DrawInstance() = 0;
    virtual void DrawMesh() = 0;
    
    virtual void ClearRenderTarget()= 0;
    
    virtual void DrawIndirect() = 0;

    
    virtual void Clear() = 0;
};
