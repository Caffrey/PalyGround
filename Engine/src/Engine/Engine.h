#pragma once
#include "Modules/Renderer/XRenderer.h"
#include "Core/World/World.h"
class XEngine
{
public:
    XEngine();
    ~XEngine();

    virtual void Tick();
    
private:
    XRenderer *Renderer;
    World* SimulationWorld;
};

static XEngine* GEngine;

