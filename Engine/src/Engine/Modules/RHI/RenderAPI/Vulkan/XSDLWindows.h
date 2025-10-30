#pragma once
#include "Modules/RHI/Windows/XWindows.h"
#include "SDL3/SDL.h"
class XSDLWindows : public XWindows
{
public:
    XSDLWindows(XEditor* Editor);
    void InitWindows() override;
    void Tick() override;
    void EventHandle() override;
    
    int Width;
    int Height;
    SDL_Window* GetWindow(){return Windows;};
private:
    SDL_Window* Windows;

    
public:
    bool IsQuit() override{return IsApplicationQuti; }
private:
    bool IsApplicationQuti;
};
