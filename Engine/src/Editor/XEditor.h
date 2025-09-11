#pragma once

#include "Engine/Modules/Windows/XWindows.h"
#include "Engine/Engine.h"

class XEditor : public XEngine
{
public:
    XEditor(XSDLWindows* Window);
    ~XEditor();

    void InitEditor();
    
};

