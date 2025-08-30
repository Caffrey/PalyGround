#pragma once


#include "Core/World/World.h"
#include "Core/World/Components/Camera.h"
class XRenderer 
{
public:
    void Render(Camera camera,World *Scene);

    void Culling();
    void RenderGbuffer();
    void RenderForwardOapque();
    void RenderPostProcessing();
    
    World* Scene;
};


