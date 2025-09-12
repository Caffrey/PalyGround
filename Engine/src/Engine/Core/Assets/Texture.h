#pragma once

#include "Core/Object.h"
#include "Modules/RHI/RHITexture.h"

class XTexture : public Object
{
public:

    RHITexture* Texture;
};
