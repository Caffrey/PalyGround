#pragma once


#include "Core/Object.h"
#include "Modules/RHI/RHIShader.h"

class XShader : public Object
{
public:

    RHIShader* VertexShader;
    RHIShader* PixelShader;
    
};
