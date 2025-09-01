#include "XRenderer.h"

void XRenderer::Render(Camera camera, World* Scene)
{
    Culling();
    RenderGbuffer();
    RenderGbuffer();
    RenderPostProcessing();
}

void XRenderer::Culling()
{
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
