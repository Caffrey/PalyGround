#pragma once
#include "Modules/Renderer/XRenderer.h"
#include "Core/World/World.h"

class XSDLWindows;
class XEngine
{
public:
    XEngine();
    ~XEngine();

    virtual void InitEngine();
    virtual void Tick(RHIInterface *RHIInterface);
    
private:
    XRenderer *Renderer;
    World* SimulationWorld;
};


