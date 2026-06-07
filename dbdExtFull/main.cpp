#include "Core/Globals.h"
#include "Core/Offsets.h"
#include "Core/Driver.h"
#include "Core/Utils.h"
#include "Core/Renderer.h"
#include "Features/ESP.h"
#include "Features/Exploits.h"
#include "Features/Workers.h"
#include "Menu/Menu.h"

inline void DrawFrame() {
    ImGui_ImplDX11_NewFrame(); ImGui_ImplWin32_NewFrame(); ImGui::NewFrame();
    Esp::Draw();
    if (globals.show_menu) RenderMenu();
    ImGui::Render();
    const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    d3d_device_ctx->OMSetRenderTargets(1, &d3d_render_target, nullptr);
    d3d_device_ctx->ClearRenderTargetView(d3d_render_target, clear_color);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    d3d_swap_chain->Present(globals.vsync ? 1 : 0, 0);
}

int main() {
    HWND consoleWnd = GetConsoleWindow(); ShowWindow(consoleWnd, SW_HIDE); FreeConsole();
    globals.build_id = GenerateRandomString(8);
    if (!Kernel->driverconnect()) return 0;

    while (Kernel->g_process_id == 0) {
        Kernel->g_process_id = Kernel->get_process_id(L"DeadByDaylight-Win64-Shipping.exe");
        if (Kernel->g_process_id != 0) { Offsets::GWorld = Offsets::GWorld_Steam; globals.current_platform = "Steam"; break; }
        Kernel->g_process_id = Kernel->get_process_id(L"DeadByDaylight-EGS-Shipping.exe");
        if (Kernel->g_process_id != 0) { Offsets::GWorld = Offsets::GWorld_Epic; globals.current_platform = "Epic Games"; break; }
        Kernel->g_process_id = Kernel->get_process_id(L"DeadByDaylight-WinGDK-Shipping.exe");
        if (Kernel->g_process_id != 0) { Offsets::GWorld = Offsets::GWorld_MS; globals.current_platform = "Xbox/MS"; break; }
        Sleep(1000);
    }
    Sleep(2000);
    globals.base_address = Kernel->get_base_address(); if (!globals.base_address) return 0;
    if (!SetupWindow()) return 0;
    if (!InitImgui()) { PerformCleanup(); return 0; }
    globals.game_hwnd = FindWindowW(L"UnrealWindow", 0);

    std::thread entity_thread(RunEntityUpdateLoop);
    std::thread local_worker_thread(RunLocalPlayerWorker);
    entity_thread.detach(); local_worker_thread.detach();

    MSG msg; ZeroMemory(&msg, sizeof(msg));
    static RECT old_rect = { 0, 0, 0, 0 }; bool insert_pressed = false;

    while (!globals.should_exit) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg); DispatchMessage(&msg);
            if (msg.message == WM_QUIT) break;
        }
        if ((GetAsyncKeyState(VK_F7) & 1) || (GetAsyncKeyState(VK_END) & 1)) globals.should_exit = true;
        if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
            if (!insert_pressed) {
                globals.show_menu = !globals.show_menu;
                LONG exStyle = GetWindowLong(globals.window_handle, GWL_EXSTYLE);
                SetWindowLong(globals.window_handle, GWL_EXSTYLE, globals.show_menu ? (exStyle & ~WS_EX_TRANSPARENT) : (exStyle | WS_EX_TRANSPARENT));
                insert_pressed = true;
            }
        } else insert_pressed = false;

        if (globals.game_hwnd && globals.window_handle) {
            RECT rect; GetWindowRect(globals.game_hwnd, &rect);
            if (rect.left != old_rect.left || rect.top != old_rect.top || rect.right != old_rect.right || rect.bottom != old_rect.bottom) {
                int width = rect.right - rect.left, height = rect.bottom - rect.top;
                if (width > 100 && height > 100) {
                    globals.ScreenWidth = width; globals.ScreenHeight = height;
                    globals.ScreenCenterX = width / 2.0f; globals.ScreenCenterY = height / 2.0f;
                    MoveWindow(globals.window_handle, rect.left, rect.top, width, height, TRUE);
                    old_rect = rect;
                }
            }
        }
        DrawFrame();
    }
    PerformCleanup();
    return 0;
}