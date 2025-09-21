#include "Modules/RHI/RenderAPI/Vulkan/XSDLWindows.h"
#include "XEditor.h"
#include <SDL3/SDL.h>

#include "VulkanInterface.h"

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

XSDLWindows::XSDLWindows(XEditor* Editor):XWindows(Editor)
{
    IsApplicationQuti = false;
}

void XSDLWindows::InitWindows()
{
    XWindows::InitWindows();

    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        // return SDL_APP_FAILURE;
    }

    Width = 1920;
    Height = 1080;

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);
    Windows = SDL_CreateWindow("SDLWindow", Width, Height, window_flags);
    auto error= SDL_GetError();

    this->Editor->InitEditor(this);
    this->RHIInterface = new VulkanRHIInterface();
    this->RHIInterface->Init(this);
}

void XSDLWindows::Tick()
{
    EventHandle();
    this->Editor->Tick(this->RHIInterface);
}



void XSDLWindows::EventHandle()
{
    SDL_Event Event;
    if(SDL_PollEvent(&Event) == true)
    {
        if(Event.type == SDL_EVENT_QUIT)
            IsApplicationQuti = true;
    }
}
