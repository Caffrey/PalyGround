#include "BgfxRHIInterface.h"
#include <Windows.h>
#include <SDL3/SDL.h>
#include <bgfx/platform.h>
#include <stdexcept>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_bgfx.h"


void BgfxRHIInterface::Init(XWindows* Window)
{
    RHIInterface::Init(Window);

    this->SDLWindow = static_cast<XSDLWindows*>(Window);

    bgfx::PlatformData pd;
    pd.nwh = SDL_GetPointerProperty(SDL_GetWindowProperties(SDLWindow->GetWindow()),SDL_PROP_WINDOW_WIN32_HWND_POINTER,nullptr);
    pd.context = nullptr;
    pd.backBuffer = nullptr;
    pd.backBufferDS = nullptr;
    bgfx::setPlatformData(pd);
    //---

    
    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.vendorId = BGFX_PCI_ID_NONE;
    init.platformData.ndt = nullptr;
    init.platformData.nwh =pd.nwh;
    init.resolution.width = this->SDLWindow->Width;
    init.resolution.height = this->SDLWindow->Height;
    if(!bgfx::init(init))
    {
        throw std::runtime_error("fuck");
    }
    //bgfx::setViewClear( 0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030FF, 1.0f, 0 );


    //Imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& IO = ImGui::GetIO();
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    
    SDL_GLContext gl_context = SDL_GL_CreateContext(SDLWindow->GetWindow());
    ImGui_Implbgfx_Init(255);
    ImGui_ImplSDL3_InitForOpenGL(SDLWindow->GetWindow(),nullptr);
    

}

void BgfxRHIInterface::BeginRenderFrame()
{
    SDL_Event Event;
    if(SDL_PollEvent(&Event) == true)
    {
        ImGui_ImplSDL3_ProcessEvent(&Event);
    }
    // for (SDL_Event current_event; SDL_PollEvent(&current_event) != 0;) {
    //     ImGui_ImplSDL3_ProcessEvent(&current_event);
    // }
    
    bgfx::setViewRect(0,0,0,1920,1080);
    
    bgfx::touch(0);
    bgfx::dbgTextClear();
    bgfx::setViewClear(0,BGFX_CLEAR_COLOR| BGFX_CLEAR_DEPTH,0xFF3030FF,1.0f,0);
    bgfx::frame();


    ImGui_Implbgfx_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    bool result = true;
    ImGui::ShowDemoWindow(&result);
    ImGui::Render();
    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
    
}

void BgfxRHIInterface::EndRenderFrame()
{
    
}


