#pragma once
#include "imgui/imgui.h"

inline void ApplyCustomTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Rounding for a modern, less blocky look
    style.WindowRounding = 8.0f;
    style.ChildRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    // Padding & Spacing for breathing room
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.FramePadding = ImVec2(10.0f, 6.0f);
    style.ItemSpacing = ImVec2(10.0f, 10.0f);
    style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 10.0f;

    // Colors (Deep Dark Theme with Crimson Accent)
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.45f, 0.45f, 0.48f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f); // Main background
    colors[ImGuiCol_ChildBg]                = ImVec4(0.11f, 0.11f, 0.12f, 1.00f); // Sidebar/Content panels
    colors[ImGuiCol_PopupBg]                = ImVec4(0.07f, 0.07f, 0.08f, 0.95f);
    colors[ImGuiCol_Border]                 = ImVec4(0.18f, 0.18f, 0.20f, 0.60f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.15f, 0.15f, 0.17f, 1.00f); // Input fields background
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.11f, 0.11f, 0.12f, 1.00f);
    
    // Accent Colors (Crimson Red)
    ImVec4 accent      = ImVec4(0.80f, 0.15f, 0.15f, 1.00f);
    ImVec4 accentHover = ImVec4(0.90f, 0.25f, 0.25f, 1.00f);
    ImVec4 accentActive= ImVec4(0.70f, 0.10f, 0.10f, 1.00f);

    colors[ImGuiCol_CheckMark]              = accent;
    colors[ImGuiCol_SliderGrab]             = accent;
    colors[ImGuiCol_SliderGrabActive]       = accentHover;
    colors[ImGuiCol_Button]                 = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = accentHover;
    colors[ImGuiCol_ButtonActive]           = accentActive;
    colors[ImGuiCol_Header]                 = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.80f, 0.15f, 0.15f, 0.60f);
    colors[ImGuiCol_HeaderActive]           = accent;
    colors[ImGuiCol_Separator]              = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = accentHover;
    colors[ImGuiCol_SeparatorActive]        = accentActive;
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered]      = accentHover;
    colors[ImGuiCol_ResizeGripActive]       = accentActive;
    colors[ImGuiCol_Tab]                    = ImVec4(0.11f, 0.11f, 0.12f, 1.00f);
    colors[ImGuiCol_TabHovered]             = accentHover;
    colors[ImGuiCol_TabActive]              = ImVec4(0.25f, 0.05f, 0.05f, 1.00f); 
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.11f, 0.11f, 0.12f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.25f, 0.05f, 0.05f, 1.00f);
}