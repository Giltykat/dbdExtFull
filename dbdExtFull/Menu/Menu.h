#pragma once
#include "../Core/Globals.h"
#include "imgui/imgui.h"
#include <cmath>

// Helper for smooth hover effects
inline float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// Custom styled checkbox with animation feel
inline bool StyledCheckbox(const char* label, bool* v) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4 accent = ImVec4(0.80f, 0.15f, 0.15f, 1.00f);
    
    bool result = ImGui::Checkbox(label, v);
    return result;
}

// Section header with separator
inline void SectionHeader(const char* title) {
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.80f, 0.15f, 0.15f, 1.00f), "%s", title);
    ImGui::Separator();
    ImGui::Spacing();
}

// Info tooltip helper
inline void InfoTooltip(const char* text) {
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", text);
    }
}

inline void RenderMenu() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Set window constraints for consistent sizing
    ImGui::SetNextWindowSizeConstraints(ImVec2(600, 500), ImVec2(800, 650));
    ImGui::SetNextWindowPos(ImVec2(globals.ScreenWidth / 2.0f - 300, globals.ScreenHeight / 2.0f - 250), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("##MasterControlInterface", &globals.show_menu, 
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {
        
        // Title Bar Area
        ImGui::PushFont(nullptr); // Use default font or custom if loaded
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - 280) * 0.5f);
        ImGui::TextColored(ImVec4(0.80f, 0.15f, 0.15f, 1.00f), "MASTER CONTROL INTERFACE");
        ImGui::PopFont();
        
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.18f, 0.18f, 0.20f, 1.00f));
        ImGui::Separator();
        ImGui::PopStyleColor();
        
        ImGui::Spacing();
        
        // Main Tab Bar
        if (ImGui::BeginTabBar("##MainTabs", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll)) {
            
            // ==================== VISUALS TAB ====================
            if (ImGui::BeginTabItem("  Visuals  ")) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
                
                // ESP Settings Group
                ImGui::BeginGroup();
                {
                    SectionHeader("[ ESP SETTINGS ]");
                    
                    ImGui::PushID("ESP_Main");
                    
                    // Aura ESP
                    ImGui::AlignTextToFramePadding();
                    ImGui::Checkbox("Aura ESP (In-Game Glow)", &config::g_auraEsp);
                    InfoTooltip("Enables the native game glow effect on entities");
                    
                    ImGui::Spacing();
                    
                    // Entity Toggles with Color Pickers
                    struct EntityToggle {
                        const char* label;
                        bool* enabled;
                        float* color;
                        const char* tooltip;
                    };
                    
                    EntityToggle entities[] = {
                        {"Survivors", &config::g_espSurvivors, config::g_colorSurvivor, "Show survivor positions"},
                        {"Killers", &config::g_espKillers, config::g_colorKiller, "Show killer positions"},
                        {"Generators", &config::g_espGenerators, config::g_colorGenerator, "Show generator locations"}
                    };
                    
                    for (auto& ent : entities) {
                        ImGui::PushID(ent.label);
                        ImGui::Checkbox(ent.label, ent.enabled);
                        InfoTooltip(ent.tooltip);
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                        ImGui::ColorEdit4("##Color", ent.color, 
                            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel);
                        InfoTooltip("Click to change color");
                        ImGui::PopID();
                    }
                    
                    ImGui::PopID();
                }
                ImGui::EndGroup();
                
                ImGui::SameLine();
                ImGui::Separator();
                ImGui::SameLine();
                
                // Display Options Group
                ImGui::BeginGroup();
                {
                    SectionHeader("[ DISPLAY OPTIONS ]");
                    
                    ImGui::PushID("DisplayOpts");
                    
                    ImGui::Checkbox("2D Box Boxes (Players)", &config::g_drawEspCornerBox);
                    InfoTooltip("Draws corner boxes around players");
                    
                    ImGui::Checkbox("Snap Lines", &config::g_drawLines);
                    InfoTooltip("Draws lines from screen center to entities");
                    
                    ImGui::Checkbox("Distance Indicators", &config::g_drawDistance);
                    InfoTooltip("Shows distance to each entity in meters");
                    
                    ImGui::Spacing();
                    
                    // Crosshair Section
                    ImGui::AlignTextToFramePadding();
                    ImGui::Checkbox("Crosshair", &config::g_drawCrosshair);
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                    ImGui::ColorEdit4("Cross Color##Crosshair", config::g_colorCrosshair,
                        ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                    InfoTooltip("Customize crosshair color");
                    
                    ImGui::PopID();
                }
                ImGui::EndGroup();
                
                ImGui::PopStyleVar();
                ImGui::EndTabItem();
            }
            
            // ==================== EXPLOITS TAB ====================
            if (ImGui::BeginTabItem("  Exploits  ")) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
                
                // Camera Mods
                {
                    SectionHeader("[ CAMERA MODIFICATIONS ]");
                    
                    ImGui::PushID("CameraMods");
                    
                    ImGui::AlignTextToFramePadding();
                    ImGui::Checkbox("FOV Changer", &config::g_fovChanger);
                    InfoTooltip("Changes your field of view");
                    
                    if (config::g_fovChanger) {
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
                        ImGui::SliderFloat("##FOVSlider", &config::g_fovValue, 80.0f, 160.0f, "%.0f FOV");
                        InfoTooltip("Recommended: 90-120 for competitive play");
                    }
                    
                    ImGui::AlignTextToFramePadding();
                    ImGui::Checkbox("Aspect Ratio (Stretched)", &config::g_aspectRatioChanger);
                    InfoTooltip("Changes aspect ratio for stretched resolution");
                    
                    if (config::g_aspectRatioChanger) {
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
                        ImGui::SliderFloat("##ARSlider", &config::g_aspectRatioValue, 1.0f, 2.5f, "%.3f");
                        InfoTooltip("1.777 = 16:9, 2.333 = 21:9 stretched");
                    }
                    
                    ImGui::PopID();
                }
                
                ImGui::Separator();
                ImGui::Spacing();
                
                // Killer Exploits
                {
                    SectionHeader("[ KILLER ABILITIES ]");
                    
                    ImGui::PushID("KillerExploits");
                    
                    ImGui::Checkbox("Infinite Lunge", &config::g_infiniteLunge);
                    InfoTooltip("Removes lunge cooldown - KILLER ONLY");
                    
                    ImGui::Checkbox("No Attack Cooldown", &config::g_noCooldown);
                    InfoTooltip("Instant attack recovery - KILLER ONLY");
                    
                    ImGui::Checkbox("Instant Bell (Wraith)", &config::g_instantBell);
                    InfoTooltip("Instantly rings the bell - WRAITH ONLY");
                    
                    ImGui::Separator();
                    ImGui::Spacing();
                    
                    // External Aura Patch
                    ImGui::Checkbox("External Aura (Always On)", &config::g_auraPatch);
                    InfoTooltip("Patches aura check function to always return 1 - makes auras visible externally");
                    
                    ImGui::PopID();
                }
                
                ImGui::Separator();
                ImGui::Spacing();
                
                // Movement Hacks
                {
                    SectionHeader("[ MOVEMENT ]");
                    
                    ImGui::PushID("Movement");
                    
                    ImGui::AlignTextToFramePadding();
                    ImGui::Checkbox("Speed Hack (Client)", &config::g_speedHack);
                    InfoTooltip("Client-side speed modification");
                    
                    if (config::g_speedHack) {
                        ImGui::SameLine();
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
                        ImGui::SliderFloat("##SpeedSlider", &config::g_speedValue, 1.0f, 1.10f, "%.2fx");
                        InfoTooltip("Values above 1.05 may be detected");
                    }
                    
                    ImGui::PopID();
                }
                
                ImGui::PopStyleVar();
                ImGui::EndTabItem();
            }
            
            // ==================== AUTOMATION TAB ====================
            if (ImGui::BeginTabItem(" Automation ")) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
                
                // Auto Skillcheck
                {
                    SectionHeader("[ AUTO SKILLCHECK ]");
                    
                    ImGui::PushID("AutoSC");
                    
                    ImGui::Checkbox("Auto-Skillcheck", &config::g_autoSkillcheck);
                    InfoTooltip("Automatically hits skillchecks perfectly");
                    
                    ImGui::TextColored(ImVec4(0.92f, 0.92f, 0.92f, 0.7f), 
                        "Notice: Hits perfectly on the line. No prefire needed.");
                    
                    ImGui::PopID();
                }
                
                ImGui::Separator();
                ImGui::Spacing();
                
                // Auto Dead Hard
                {
                    SectionHeader("[ AUTO DEAD HARD ]");
                    
                    ImGui::PushID("AutoDH");
                    
                    // Status Indicator
                    ImVec4 statusColor = config::g_autoDeadHard ? 
                        ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                    const char* statusText = config::g_autoDeadHard ? 
                        "STATUS: ACTIVE (Listening for swings)" : "STATUS: INACTIVE";
                    
                    ImGui::PushStyleColor(ImGuiCol_Text, statusColor);
                    ImGui::Text("%s", statusText);
                    ImGui::PopStyleColor();
                    
                    ImGui::Spacing();
                    
                    // Key Configuration
                    ImGui::Text("Hold/Toggle Hotkey (VK Code):");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                    ImGui::InputInt("##ToggleKey", &config::g_autoDeadHardKey, 0, 0);
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "(Default: 5 = Mouse Button 4)");
                    
                    ImGui::Text("In-Game Dead Hard Key:");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                    ImGui::InputInt("##DHKey", &config::g_deadHardKey, 0, 0);
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "(Example: 69 = 'E')");
                    
                    ImGui::PopID();
                }
                
                ImGui::PopStyleVar();
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
        
        // Footer
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        // Hotkey Info Bar
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::Text("Hotkeys:");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.15f, 0.15f, 1.00f));
        ImGui::Text("[INSERT]");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::Text("Menu |");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.15f, 0.15f, 1.00f));
        ImGui::Text("[END]");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::Text("Kill Cheat");
        ImGui::PopStyleColor();
        
        // Version/Build Info (optional)
        ImGui::SameLine(ImGui::GetWindowWidth() - 120);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::Text("v2.0 PRO");
        ImGui::PopStyleColor();
        
    }
    ImGui::End();
}