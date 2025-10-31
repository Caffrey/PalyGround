#include "BgfxRHIBuffer.h"

void BgfxRHIVertexBuffer::UpdateVertexBuffer(RHIVertexStreamComponent VertexStream)
{
    bgfx::destroy(VertexHandle);

    VertexLayout = bgfx::VertexLayout();
    VertexLayout.begin();
    
    if(VertexStream.HaveLayout(RHIVertexLayoutType::Position))
    {
        VertexLayout.add(bgfx::Attrib::Position,3,bgfx::AttribType::Float);
    }
    if(VertexStream.HaveLayout(RHIVertexLayoutType::Normal))
    {
        VertexLayout.add(bgfx::Attrib::Normal,3,bgfx::AttribType::Float);
    }
    if(VertexStream.HaveLayout(RHIVertexLayoutType::Tangent))
    {
        VertexLayout.add(bgfx::Attrib::Tangent,4,bgfx::AttribType::Float);
    }
    if(VertexStream.HaveLayout(RHIVertexLayoutType::Bitangent))
    {
        VertexLayout.add(bgfx::Attrib::Bitangent,4,bgfx::AttribType::Float);
    }
    if(VertexStream.HaveLayout(RHIVertexLayoutType::Color))
    {
        VertexLayout.add(bgfx::Attrib::Color0,4,bgfx::AttribType::Float);
    }
    if(VertexStream.HaveLayout(RHIVertexLayoutType::TexCoord0))
    {
        VertexLayout.add(bgfx::Attrib::TexCoord0,4,bgfx::AttribType::Float);
    }
    if(VertexStream.HaveLayout(RHIVertexLayoutType::TexCoord1))
    {
        VertexLayout.add(bgfx::Attrib::TexCoord1,4,bgfx::AttribType::Float);
    }
    if(VertexStream.HaveLayout(RHIVertexLayoutType::TexCoord2))
    {
        VertexLayout.add(bgfx::Attrib::TexCoord2,4,bgfx::AttribType::Float);
    }
    if(VertexStream.HaveLayout(RHIVertexLayoutType::TexCoord3))
    {
        VertexLayout.add(bgfx::Attrib::TexCoord3,4,bgfx::AttribType::Float);
    }
    if(VertexStream.HaveLayout(RHIVertexLayoutType::TexCoord4))
    {
        VertexLayout.add(bgfx::Attrib::TexCoord4,4,bgfx::AttribType::Float);
    }
    if(VertexStream.HaveLayout(RHIVertexLayoutType::TexCoord5))
    {
        VertexLayout.add(bgfx::Attrib::TexCoord5,4,bgfx::AttribType::Float);
    }
    VertexLayout.end();
    
    VertexHandle = bgfx::createVertexBuffer(bgfx::makeRef(VertexStream.Data,VertexStream.GetDataLegnth()),VertexLayout);
}

void BgfxRHIIndexBuffer::CreateIndexBuffer(void* Indices, int Length)
{
    IndexHandle = bgfx::createIndexBuffer(bgfx::makeRef(Indices,Length));
}
