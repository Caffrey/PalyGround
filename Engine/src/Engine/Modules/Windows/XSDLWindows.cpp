#include "Modules/Windows/XSDLWindows.h"

#include <SDL3/SDL.h>

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

void XSDLWindows::InitWindows()
{
    XWindows::InitWindows();

    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        // return SDL_APP_FAILURE;
    }

    Width = 640;
    Height = 480;
    Windows = SDL_CreateWindow("SDLWindow", Width, Height, SDL_WINDOW_OPENGL);
    
    glContext = SDL_GL_CreateContext(Windows);

    Engine = new XEngine();
    GEngine = Engine;
}

void XSDLWindows::Tick()
{
    XWindows::Tick();
    GEngine->Tick();
}

void XSDLWindows::EventHandle()
{
    XWindows::EventHandle();
}
