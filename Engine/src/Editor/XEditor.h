#pragma once

#include "Engine/Modules/Windows/XWindows.h"
#include "Engine/Engine.h"

class XEditor
{
public:
    XEditor();
    ~XEditor();
    void InitlizedEditor();

    XWindows *Window;
    XEngine * Engine;
};

static XEditor* GEditor = nullptr;