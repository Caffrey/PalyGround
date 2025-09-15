#pragma once
#include <cstdint>

#include "RHICommon.h"

class RHIBuffer : public RHIObject
{
public:

    
    virtual void Destory(){};
    
};

struct RHIBufferDesc
{
    uint32_t Size{};
    uint32_t Stride{};

    RHIBufferDesc() = default;
    RHIBufferDesc(uint32_t InSize, uint32_t InStride)
        : Size  (InSize)
        , Stride(InStride)
    {}

    static RHIBufferDesc Null()
    {
        return RHIBufferDesc(0, 0);
    }

    bool IsNull() const
    {
        if((Size == 0 && Stride == 0))
            return true;

        return false;
    }
};

class RHIVertexBuffer : public RHIBuffer
{
public:
    void CreateVertexBuffer(RHIBufferDesc* BufferDesc){};
    void MappingVertexData(void* Verteices, void* Indices){};
protected:
    uint32_t VertexSize;
    uint32_t IndicesSize;
};

//Const Buffer?