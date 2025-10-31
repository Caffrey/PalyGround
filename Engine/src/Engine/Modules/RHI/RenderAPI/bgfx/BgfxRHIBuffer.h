#pragma once
#include <bgfx/bgfx.h>

#include "Modules/RHI/RHIBuffer.h"


class BgfxRHIVertexBuffer : public RHIVertexBuffer2
{
public:
    void UpdateVertexBuffer(RHIVertexStreamComponent VertexStream) override;

protected:
    bgfx::VertexLayout VertexLayout;
    bgfx::VertexBufferHandle VertexHandle;
};

class BgfxRHIIndexBuffer:public RHIIndexBuffer
{
public:
    BgfxRHIIndexBuffer():RHIIndexBuffer(){};
    // 1,2,3 / size of Array
    void CreateIndexBuffer(void* Indices, int Length) override;
protected:
    bgfx::IndexBufferHandle IndexHandle;
};