#pragma once
#include "XWindows.h"
#include "SDL3/SDL.h"
#include "Engine/Engine.h"

class XSDLWindows : public XWindows
{
public:
    XSDLWindows(){};
    void InitWindows() override;
    void Tick() override;
    void EventHandle() override;

    
    int Width;
    int Height;
    SDL_Window* GetWindow(){return Windows;};
private:
    SDL_Window* Windows;
    SDL_GLContext glContext;
    XEngine * Engine;


};
