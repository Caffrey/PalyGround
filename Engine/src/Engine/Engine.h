#pragma once
#include "Modules/Renderer/XRenderer.h"
#include "Core/World/World.h"

class XSDLWindows;
class XEngine
{
public:
    XEngine(XSDLWindows* Window);
    ~XEngine();

    virtual void InitEngine();
    virtual void Tick();
    
private:
    XRenderer *Renderer;
    World* SimulationWorld;
    RHIInterface* RHIInterface;
    
};


