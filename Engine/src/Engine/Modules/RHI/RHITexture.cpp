#include "RHITexture.h"

#include <bgfx/bgfx.h>


void RHITexture2D::CreateTexture(RHITextureDesc& TextureDesc, void* Data, int Length)
{
    const bgfx::Memory* memory = bgfx::makeRef(Data , Length);

    auto type =(bgfx::TextureFormat::Enum)(int)TextureDesc.TextureFormat;
    bgfx::createTexture2D(TextureDesc.Width,TextureDesc.Height,TextureDesc.Mip,1,type,0,memory);
}
