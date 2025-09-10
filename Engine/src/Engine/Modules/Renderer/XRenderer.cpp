#include "XRenderer.h"
#include "Modules/Windows/XSDLWindows.h"
#include <SDL3/SDL_vulkan.h>

void XRenderer::Render(Camera* camera, World* Scene)
{
    Culling();
    RenderGbuffer();
    RenderGbuffer();
    RenderPostProcessing();
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
