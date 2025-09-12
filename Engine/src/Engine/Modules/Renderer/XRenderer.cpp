#include "XRenderer.h"
#include <SDL3/SDL_vulkan.h>

void XRenderer::Render(Camera* camera, World* Scene, RHIInterface* RHIInterface)
{
    RHIInterface->BeginRenderFrame();
    Culling();
    RenderGbuffer();
    RenderGbuffer();
    RenderPostProcessing();
    RHIInterface->EndRenderFrame();
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
