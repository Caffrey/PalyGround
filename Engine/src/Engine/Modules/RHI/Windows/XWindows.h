#pragma once
#include "Modules/RHI/RHIInterface.h"
class XEditor;
class XWindows
{
    
public:
    XWindows(XEditor* Editor);
    virtual void InitWindows(){};
    virtual void Tick(){};
    virtual void EventHandle(){};

public:
   virtual bool IsQuit(){return false;}

    
protected:
    XEditor* Editor;
    RHIInterface* RHIInterface;
};



