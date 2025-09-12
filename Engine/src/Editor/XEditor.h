#pragma once

#include "Engine/Engine.h"
class XWindows;
class XEditor : public XEngine
{
public:
    XEditor();
    ~XEditor();

    void InitEditor(XWindows* Window);
    
};

