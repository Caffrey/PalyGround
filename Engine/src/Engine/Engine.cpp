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
    Renderer->Render(nullptr,SimulationWorld);
    //RenderSystem
    // Renderer->Render();
}
