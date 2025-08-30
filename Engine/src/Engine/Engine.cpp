#include "Engine.h"

XEngine::XEngine()
{
    GEngine = this;
    Renderer = new XRenderer();
}

XEngine::~XEngine()
{
    GEngine = nullptr;
}

void XEngine::Tick()
{
    Renderer->Render();
}
