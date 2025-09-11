#include "XRenderer.h"
#include "Modules/Windows/XSDLWindows.h"
#include <SDL3/SDL_vulkan.h>

void XRenderer::Render(Camera* camera, World* Scene, RHIInterface* RHIInterface)
{
    Culling();
    RenderGbuffer();
    RenderGbuffer();
    RenderPostProcessing();
    RHIInterface->BeginDraw();
    RHIInterface->EndDraw();
}

void XRenderer::Culling()
{
    VkInstance vk;
}

void XRenderer::RenderGbuffer()
{
}

void XRenderer::RenderForwardOapque()
{
}

void XRenderer::RenderPostProcessing()
{
}
