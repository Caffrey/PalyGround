#include "Engine.h"
#include "Modules/Windows/XSDLWindows.h"

XEngine::XEngine(XSDLWindows* Window)
{
    RHIInterface = new VulkanRHIInterface();
    RHIInterface->Init(Window);
    
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

void XEngine::Tick()
{
    SimulationWorld->Tick();
    SimulationWorld->PhysicalTick();
    SimulationWorld->RenderWorld();
    Renderer->Render(nullptr,SimulationWorld,this->RHIInterface);
}
