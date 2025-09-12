#pragma once

#include "XEditor.h"
#include "Modules/RHI/RenderAPI/Vulkan/XSDLWindows.h"
int main(int argc, char *argv[])
{
    XEditor* Editor = new XEditor();
    XSDLWindows* windows = new  XSDLWindows(Editor);
    windows->InitWindows();
    
    while (true)
    {
        windows->Tick();
        if(windows->IsQuit())
            break;
    }
    return 0;
}