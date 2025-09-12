#include "Engine.h"

XEngine::XEngine()
{
}
static XEngine* GEngine = nullptr;

XEngine::~XEngine()
{
}

void XEngine::InitEngine()
{
    Renderer = new XRenderer();
    SimulationWorld = new World();
    GEngine = this;
}

void XEngine::Tick(RHIInterface *RHIInterface)
{
    SimulationWorld->Tick();
    SimulationWorld->PhysicalTick();
    SimulationWorld->RenderWorld();
    Renderer->Render(nullptr,SimulationWorld,RHIInterface);
}
