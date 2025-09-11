#include "Editor/XEditor.h"
#include "Engine/Modules/Windows/XSDLWindows.h"


XEditor* GEditor = nullptr;

XEditor::XEditor(XSDLWindows* Window):XEngine(Window)
{
}

XEditor::~XEditor()
{
}

void XEditor::InitEditor()
{
    InitEngine();
    GEditor = this;
}

