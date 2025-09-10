#include "Editor/XEditor.h"
#include "Engine/Modules/Windows/XSDLWindows.h"

XEditor::XEditor()
{
}

XEditor::~XEditor()
{
}

void XEditor::InitlizedEditor()
{
    Window = new XSDLWindows();
    Window->InitWindows();
}

void XEditor::Tick()
{
    Window->Tick();
}
