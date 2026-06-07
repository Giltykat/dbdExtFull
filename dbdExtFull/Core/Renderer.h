#pragma once
#include "Globals.h"
#include "Offsets.h"
#include "Driver.h"
#include "Utils.h"
#include <dwmapi.h>
#include <dxgi.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwmapi.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#ifndef WDA_EXCLUDEFROMCAPTURE
#define WDA_EXCLUDEFROMCAPTURE 0x00000011
#endif

inline D3DMATRIX CreateMatrix(SDK::FRotator rot, SDK::FVector origin) {
    const double DEG_TO_RAD = 3.14159265358979323846 / 180.0;
    float SP = sin(rot.Pitch * DEG_TO_RAD), CP = cos(rot.Pitch * DEG_TO_RAD);
    float SY = sin(rot.Yaw * DEG_TO_RAD), CY = cos(rot.Yaw * DEG_TO_RAD);
    float SR = sin(rot.Roll * DEG_TO_RAD), CR = cos(rot.Roll * DEG_TO_RAD);
    D3DMATRIX matrix;
    matrix.m[0][0] = CP * CY; matrix.m[0][1] = CP * SY; matrix.m[0][2] = SP; matrix.m[0][3] = 0.f;
    matrix.m[1][0] = SR * SP * CY - CR * SY; matrix.m[1][1] = SR * SP * SY + CR * CY; matrix.m[1][2] = -SR * CP; matrix.m[1][3] = 0.f;
    matrix.m[2][0] = -(CR * SP * CY + SR * SY); matrix.m[2][1] = CY * SR - CR * SP * SY; matrix.m[2][2] = CR * CP; matrix.m[2][3] = 0.f;
    matrix.m[3][0] = origin.X; matrix.m[3][1] = origin.Y; matrix.m[3][2] = origin.Z; matrix.m[3][3] = 1.f;
    return matrix;
}

class Overlay {
public:
    SDK::FVector WorldToScreen(const SDK::FMinimalViewInfo& camera, const SDK::FVector& WorldLocation) {
        SDK::FVector Screenlocation;
        if (camera.FOV < 5.0f || camera.FOV > 170.0f) return SDK::FVector(0, 0, 0);
        D3DMATRIX tempMatrix = CreateMatrix(camera.Rotation, SDK::FVector(0, 0, 0));
        SDK::FVector vAxisX(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
        SDK::FVector vAxisY(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
        SDK::FVector vAxisZ(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
        SDK::FVector vDelta(WorldLocation.X - camera.Location.X, WorldLocation.Y - camera.Location.Y, WorldLocation.Z - camera.Location.Z);
        double dX = vDelta.Dot(vAxisY), dY = vDelta.Dot(vAxisZ), dZ = vDelta.Dot(vAxisX);
        if (dZ < 1.0) dZ = 1.0;
        const double FOV_DEG_TO_RAD = 3.14159265358979323846 / 360.0;
        Screenlocation.X = globals.ScreenCenterX + dX * (globals.ScreenCenterX / tan(camera.FOV * FOV_DEG_TO_RAD)) / dZ;
        Screenlocation.Y = globals.ScreenCenterY - dY * (globals.ScreenCenterX / tan(camera.FOV * FOV_DEG_TO_RAD)) / dZ;
        return Screenlocation;
    }
};

class Render {
public:
    void DrawCornerBox(ImDrawList* drawList, float x, float y, float width, float height, ImColor color, float thickness, float cornerRatio) {
        if (!drawList) return;
        float lineW = width * cornerRatio, lineH = height * cornerRatio;
        drawList->AddLine(ImVec2(x, y), ImVec2(x + lineW, y), color, thickness);
        drawList->AddLine(ImVec2(x, y), ImVec2(x, y + lineH), color, thickness);
        drawList->AddLine(ImVec2(x + width, y), ImVec2(x + width - lineW, y), color, thickness);
        drawList->AddLine(ImVec2(x + width, y), ImVec2(x + width, y + lineH), color, thickness);
        drawList->AddLine(ImVec2(x, y + height), ImVec2(x + lineW, y + height), color, thickness);
        drawList->AddLine(ImVec2(x, y + height), ImVec2(x, y + height - lineH), color, thickness);
        drawList->AddLine(ImVec2(x + width, y + height), ImVec2(x + width - lineW, y + height), color, thickness);
        drawList->AddLine(ImVec2(x + width, y + height), ImVec2(x + width, y + height - lineH), color, thickness);
    }
    void DrawCrosshair(ImColor color) {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        float x = globals.ScreenCenterX, y = globals.ScreenCenterY, size = 8.0f;
        drawList->AddLine(ImVec2(x - size, y), ImVec2(x + size, y), ImColor(0, 0, 0, 255), 3.0f);
        drawList->AddLine(ImVec2(x, y - size), ImVec2(x, y + size), ImColor(0, 0, 0, 255), 3.0f);
        drawList->AddLine(ImVec2(x - size, y), ImVec2(x + size, y), color, 1.0f);
        drawList->AddLine(ImVec2(x, y - size), ImVec2(x, y + size), color, 1.0f);
    }
};

class Cheat {
public:
    std::vector<Entity> actors;
    LocalPlayer GetLocalPlayer() {
        LocalPlayer lp = {}; if (!globals.base_address) return lp;
        uintptr_t uworld = Kernel->read<uintptr_t>(globals.base_address + Offsets::GWorld);
        uintptr_t game_instance = Kernel->read<uintptr_t>(uworld + Offsets::OwningGameInstance);
        uintptr_t local_players_array = Kernel->read<uintptr_t>(game_instance + Offsets::LocalPlayers);
        uintptr_t local_player = Kernel->read<uintptr_t>(local_players_array);
        uintptr_t player_controller = Kernel->read<uintptr_t>(local_player + Offsets::PlayerController);
        uintptr_t cameraManager = Kernel->read<uintptr_t>(player_controller + Offsets::PlayerCameraManager);
        lp.FMinimalViewInfo = Kernel->read<SDK::FMinimalViewInfo>(cameraManager + Offsets::CameraCachePrivate + 0x10);
        return lp;
    }
};

namespace global {
    inline Overlay* overlay = new Overlay();
    inline Render* render = new Render();
    inline Cheat* m_cheat = new Cheat();
}

inline ID3D11Device* d3d_device = nullptr;
inline ID3D11DeviceContext* d3d_device_ctx = nullptr;
inline IDXGISwapChain* d3d_swap_chain = nullptr;
inline ID3D11RenderTargetView* d3d_render_target = nullptr;

inline LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;
    if (msg == WM_DESTROY) { PostQuitMessage(0); return 0; }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

inline void PerformCleanup() {
    globals.should_exit = true; std::this_thread::sleep_for(std::chrono::milliseconds(250));
    if (ImGui::GetCurrentContext()) { ImGui_ImplDX11_Shutdown(); ImGui_ImplWin32_Shutdown(); ImGui::DestroyContext(); }
    if (d3d_render_target) { d3d_render_target->Release(); d3d_render_target = nullptr; }
    if (d3d_swap_chain) { d3d_swap_chain->Release(); d3d_swap_chain = nullptr; }
    if (d3d_device_ctx) { d3d_device_ctx->Release(); d3d_device_ctx = nullptr; }
    if (d3d_device) { d3d_device->Release(); d3d_device = nullptr; }
    if (globals.window_handle) { DestroyWindow(globals.window_handle); UnregisterClassA(globals.rand_class_name.c_str(), GetModuleHandle(nullptr)); }
}

inline bool InitImgui() {
    DXGI_SWAP_CHAIN_DESC sd; ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2; sd.BufferDesc.Width = globals.ScreenWidth; sd.BufferDesc.Height = globals.ScreenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; sd.OutputWindow = globals.window_handle;
    sd.SampleDesc.Count = 1; sd.Windowed = TRUE; sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    const D3D_FEATURE_LEVEL lvl[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, lvl, 2, D3D11_SDK_VERSION, &sd, &d3d_swap_chain, &d3d_device, NULL, &d3d_device_ctx))) return false;
    ID3D11Texture2D* pBack; d3d_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBack));
    d3d_device->CreateRenderTargetView(pBack, NULL, &d3d_render_target); pBack->Release();
    IMGUI_CHECKVERSION(); ImGui::CreateContext(); ImGui::GetIO().IniFilename = nullptr;
    ImGui_ImplWin32_Init(globals.window_handle); ImGui_ImplDX11_Init(d3d_device, d3d_device_ctx);
    return true;
}

inline bool SetupWindow() {
    globals.ScreenWidth = GetSystemMetrics(SM_CXSCREEN); globals.ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    globals.ScreenCenterX = globals.ScreenWidth / 2.0f; globals.ScreenCenterY = globals.ScreenHeight / 2.0f;
    globals.rand_class_name = GenerateRandomString(16); globals.rand_window_name = GenerateRandomString(16);
    WNDCLASSEXA wc = { sizeof(WNDCLASSEXA), CS_CLASSDC, WindowProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, globals.rand_class_name.c_str(), nullptr };
    RegisterClassExA(&wc);
    globals.window_handle = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        globals.rand_class_name.c_str(), globals.rand_window_name.c_str(), WS_POPUP, 0, 0, globals.ScreenWidth, globals.ScreenHeight, nullptr, nullptr, wc.hInstance, nullptr);
    if (!globals.window_handle) return false;
    SetLayeredWindowAttributes(globals.window_handle, RGB(0, 0, 0), 255, LWA_ALPHA);
    MARGINS margin = { -1, -1, -1, -1 }; DwmExtendFrameIntoClientArea(globals.window_handle, &margin);
    SetWindowDisplayAffinity(globals.window_handle, WDA_EXCLUDEFROMCAPTURE);
    ShowWindow(globals.window_handle, SW_SHOW); UpdateWindow(globals.window_handle);
    return true;
}