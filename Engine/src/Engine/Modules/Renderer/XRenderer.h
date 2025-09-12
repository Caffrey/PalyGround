#pragma once

#include "Modules/RHI/RHIInterface.h"
#include "Core/World/World.h"
#include "Core/World/Components/Camera.h"
class XRenderer 
{
public:
    void Render(Camera* camera,World *Scene, RHIInterface* RHIInterface);

    void Culling();
    void RenderGbuffer();
    void RenderForwardOapque();
    void RenderPostProcessing();
    
    World* Scene;
};


