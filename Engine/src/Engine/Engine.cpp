#include "Engine.h"

XEngine::XEngine()
{
    Renderer = new XRenderer();
    SimulationWorld = new World();
}

XEngine::~XEngine()
{
}

void XEngine::Tick()
{
    SimulationWorld->Tick();
    SimulationWorld->PhysicalTick();
    SimulationWorld->RenderWorld();
    //RenderSystem
    // Renderer->Render();
}
