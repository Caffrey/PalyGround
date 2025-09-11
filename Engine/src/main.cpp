#pragma once

#include "XEditor.h"
#include "Engine/Modules/Windows/XSDLWindows.h"
int main(int argc, char *argv[])
{
    XSDLWindows* windows = new  XSDLWindows();
    windows->InitWindows();

    XEditor* Editor = new XEditor(windows);
    Editor->InitEditor();
    
    while (true)
    {
        windows->Tick();
        Editor->Tick();
    }
    return 0;
}