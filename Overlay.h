#pragma once
#include "LeagueMemoryReader.h"

#include <chrono>
#include <map>
#include <list>

#include "windows.h"
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_dx11.h"
#include "external/imgui/imgui_impl_win32.h"

#include "MenuConfig.h"

#include <dinput.h>
#include <dxgi1_3.h>
#include <d3d11_2.h>
#include <dcomp.h>


using namespace std::chrono;

/// Manages the overlay of the cheat. Also manages the executing scripts.
class Overlay {

public:
    //Overlay();
    void         Init();                          
    void         StartFrame();
    ImDrawList*  GetDrawList();
    void         RenderFrame();
                 
    bool         IsVisible();
    void         Hide();
    void         Show();
    void         ToggleTransparent();

    void         DrawMenu(MenuConfig &menuconfig);
    
    static ID3D11Device*  GetDxDevice();

	float renderTimeMs;

private:
    void  DrawScriptCommonSettings(int id);
    void  DrawScriptSettings();

    static bool CreateDeviceD3D(HWND hWnd);
    static void CleanupDeviceD3D();
    static void CreateRenderTarget();
    static void CleanupRenderTarget();
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


private: 
    HWND hWindow;
    bool isWindowVisible = true;

    // DirectX stuff
    static ID3D11Device*           dxDevice;
    static ID3D11DeviceContext*    dxDeviceContext;
    static IDXGISwapChain1*        dxSwapChain;
    static ID3D11RenderTargetView* dxRenderTarget;


};