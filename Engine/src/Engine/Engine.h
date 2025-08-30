#pragma once
#include "Renderer/XRenderer.h"
class XEngine
{
public:
    XEngine();
    ~XEngine();

    virtual void Tick();
    
private:
    XRenderer *Renderer;
};

static XEngine* GEngine;

