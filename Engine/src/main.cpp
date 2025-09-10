#pragma once

#include "Engine/Modules/Windows/XSDLWindows.h"
int main(int argc, char *argv[])
{
    XSDLWindows* windows = new  XSDLWindows();

    windows->InitWindows();

    while (true)
    {
        windows->Tick();
    }
    return 0;
}