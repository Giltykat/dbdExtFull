#pragma once
#include "../Core/Globals.h"
#include "imgui/imgui.h"

inline void RenderMenu() {
    ImGui::SetNextWindowSize(ImVec2(520, 440), ImGuiCond_Once);
    if (ImGui::Begin("Master Control Interface", &globals.show_menu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        if (ImGui::BeginTabBar("Tabs")) {
            if (ImGui::BeginTabItem("Visuals (ESP)")) {
                ImGui::Checkbox("Aura ESP (In-Game Glow)", &config::g_auraEsp); ImGui::Separator();
                ImGui::Checkbox("Survivors", &config::g_espSurvivors); ImGui::SameLine(180); ImGui::ColorEdit3("##colSurv", config::g_colorSurvivor, ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox("Killers", &config::g_espKillers); ImGui::SameLine(180); ImGui::ColorEdit3("##colKill", config::g_colorKiller, ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox("Generators", &config::g_espGenerators); ImGui::SameLine(180); ImGui::ColorEdit3("##colGen", config::g_colorGenerator, ImGuiColorEditFlags_NoInputs);
                ImGui::Separator();
                ImGui::Checkbox("Draw 2D Box (Players)", &config::g_drawEspCornerBox);
                ImGui::Checkbox("Draw Snaplines", &config::g_drawLines);
                ImGui::Checkbox("Draw Distance", &config::g_drawDistance);
                ImGui::Checkbox("Crosshair", &config::g_drawCrosshair); ImGui::SameLine(); ImGui::ColorEdit3("Cross Color", config::g_colorCrosshair, ImGuiColorEditFlags_NoInputs);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Exploits")) {
                ImGui::Checkbox("FOV Changer", &config::g_fovChanger);
                if (config::g_fovChanger) ImGui::SliderFloat("FOV Value", &config::g_fovValue, 80.0f, 160.0f, "%.1f");
                ImGui::Checkbox("Aspect Ratio (Stretched Res)", &config::g_aspectRatioChanger);
                if (config::g_aspectRatioChanger) ImGui::SliderFloat("Aspect Ratio", &config::g_aspectRatioValue, 1.0f, 2.5f, "%.3f");
                ImGui::Separator();
                ImGui::Checkbox("Infinite Lunge (Killer)", &config::g_infiniteLunge);
                ImGui::Checkbox("No Attack Cooldown (Killer)", &config::g_noCooldown);
                ImGui::Checkbox("Instant Bell (WRAITH ONLY)", &config::g_instantBell);
                ImGui::Separator();
                ImGui::Checkbox("Speed Hack (Client-Side)", &config::g_speedHack);
                ImGui::SliderFloat("Speed Value", &config::g_speedValue, 1.0f, 1.10f, "%.2f");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Automation")) {
                ImGui::Checkbox("Auto-Skillcheck", &config::g_autoSkillcheck);
                ImGui::Text("Notice: Hits perfectly on the line. No prefire needed."); ImGui::Separator();
                ImGui::Text("Auto Dead Hard Settings:");
                ImGui::TextColored(config::g_autoDeadHard ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), config::g_autoDeadHard ? "STATUS: ACTIVE (Listening for swings)" : "STATUS: INACTIVE");
                ImGui::Text("Hold/Toggle Hotkey (VK Code):"); ImGui::InputInt("##toggleKey", &config::g_autoDeadHardKey); ImGui::SameLine(); ImGui::Text("(Default 5 = Mouse Button 4)");
                ImGui::Text("In-Game Dead Hard Key (e.g. 69 for 'E'):"); ImGui::InputInt("##dhKey", &config::g_deadHardKey);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Hotkeys: [INSERT] Menu | [END] Kill Cheat");
    }
    ImGui::End();
}