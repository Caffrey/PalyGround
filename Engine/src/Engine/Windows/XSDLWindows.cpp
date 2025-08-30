#include "Windows/XSDLWindows.h"

#include "glad/gl.h"
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
    Windows = SDL_CreateWindow("SDLWindow", 640, 480, SDL_WINDOW_OPENGL);
    glContext = SDL_GL_CreateContext(Windows);

    Engine = new XEngine();
}

void XSDLWindows::Tick()
{
    XWindows::Tick();
}

void XSDLWindows::EventHandle()
{
    XWindows::EventHandle();
}
