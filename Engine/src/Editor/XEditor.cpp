#include "Editor/XEditor.h"

XEditor* GEditor = nullptr;


void XEditor::InitEditor(XWindows* Window)
{
    InitEngine();
    GEditor = this;
}

XEditor::XEditor()
{
    
}

XEditor::~XEditor()
{
}
