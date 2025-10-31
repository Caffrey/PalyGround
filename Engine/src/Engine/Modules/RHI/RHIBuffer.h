#pragma once
#include <cstdint>
#include <vector>

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


enum RHIVertexLayoutType
{
    None = 0,
    Position = 1 << 0,  
    Normal= 1 << 1,    
    Tangent= 1 << 2,   
    Bitangent= 1 << 3, 
    Color=1 << 4,     
    TexCoord0=1 << 5, 
    TexCoord1=1 << 6, 
    TexCoord2=1 << 7, 
    TexCoord3=1 << 8, 
    TexCoord4=1 << 9, 
    TexCoord5=1 << 10,
};


struct RHIVertexStreamComponent
{
public:
    RHIVertexStreamComponent();
    ~RHIVertexStreamComponent();
    int GetStride(){return Stride;}
    void UpdateLayout(RHIVertexLayoutType InFlag, int InVertexCount);
    virtual void UploadMeshToGPU();
    bool GetLayoutOffset(RHIVertexLayoutType Flag, int& Offset);
    void GetLayoutPointer(RHIVertexLayoutType Flag, float** StartIndex);
    RHIVertexLayoutType GetLayout(){return Layout;};
    bool HaveLayout(RHIVertexLayoutType Flag);

    int GetDataLegnth(){return DataLength;}
    float* Data;

    bool IsValid(){return Stride != 0 && VertexCount != 0 &&Data != nullptr && Layout > 0;}
protected:
    int Stride;
    int VertexCount;
    int DataLength;
    bool CalLayoutLength(RHIVertexLayoutType Flag,int& Length);
    
    RHIVertexLayoutType Layout;
};


struct RHIVertexLayoutDesc
{
public:
    bool Position;  
    bool Normal;    
    bool Tangent;   
    bool Bitangent; 
    bool Color;     
    bool TexCoord0; 
    bool TexCoord1; 
    bool TexCoord2; 
    bool TexCoord3; 
    bool TexCoord4; 
    bool TexCoord5; 
};

class RHIVertexBuffer2 : public RHIBuffer
{
public:
    RHIVertexBuffer2(){};
    //Position Layout {-1.0f,  1.0f,  1.0f, 0xff000000 },Pos,Color/Pos,Color
    virtual void UpdateVertexBuffer(RHIVertexStreamComponent VertexStream){};

};

class RHIIndexBuffer : public RHIBuffer
{
public:
    RHIIndexBuffer(){};
    virtual void CreateIndexBuffer(void *Vertices, int Length){};

};

//Const Buffer?