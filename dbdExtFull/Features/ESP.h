#pragma once
#include "../Core/Renderer.h"

namespace Esp {
    inline void Draw2DBox(SDK::FVector pos, const SDK::FMinimalViewInfo& cam, ImColor color) {
        SDK::FVector head = { pos.X, pos.Y, pos.Z + 100.0 }; pos.Z -= 100.0;
        SDK::FVector head2D = global::overlay->WorldToScreen(cam, head);
        SDK::FVector feet2D = global::overlay->WorldToScreen(cam, pos);
        double boxHeight = feet2D.Y - head2D.Y, boxWidth = boxHeight / 2.4;
        double topX = head2D.X - boxWidth / 2.0, topY = head2D.Y;
        global::render->DrawCornerBox(ImGui::GetBackgroundDrawList(), (float)topX, (float)topY, (float)boxWidth, (float)boxHeight, color, 1.5f, 0.25f);
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2((float)topX, (float)topY), ImVec2((float)(topX + boxWidth), (float)(topY + boxHeight)), ImColor(0, 0, 0, 60));
    }

    inline void DrawLines(SDK::FVector pos, const SDK::FMinimalViewInfo& cam, ImColor color) {
        SDK::FVector head = { pos.X, pos.Y, pos.Z + 100.0 };
        SDK::FVector head2D = global::overlay->WorldToScreen(cam, head);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(globals.ScreenCenterX, globals.ScreenHeight * 1.0f), ImVec2((float)head2D.X, (float)head2D.Y), color, 1.f);
    }

    inline void DrawNameAndDistance(SDK::FVector pos, const SDK::FMinimalViewInfo& cam, const std::string& name, ImColor color, EntityType type) {
        double zOffset = (type == EntityType::Survivor || type == EntityType::Killer) ? 100.0 : 40.0;
        SDK::FVector head = { pos.X, pos.Y, pos.Z + zOffset };
        SDK::FVector head2D = global::overlay->WorldToScreen(cam, head);
        if (config::g_drawName) {
            float textWidth = ImGui::CalcTextSize(name.c_str()).x;
            ImGui::GetBackgroundDrawList()->AddText(ImVec2(head2D.X - textWidth / 2.f + 1, head2D.Y - 15.f), ImColor(0, 0, 0, 200), name.c_str());
            ImGui::GetBackgroundDrawList()->AddText(ImVec2(head2D.X - textWidth / 2.f, head2D.Y - 16.f), color, name.c_str());
        }
        if (config::g_drawDistance) {
            std::string distStr = "[" + std::to_string((int)(cam.Location.Distance(pos) / 100.0)) + "m]";
            float distWidth = ImGui::CalcTextSize(distStr.c_str()).x;
            ImGui::GetBackgroundDrawList()->AddText(ImVec2(head2D.X - distWidth / 2.f + 1, head2D.Y - 29.f), ImColor(0, 0, 0, 200), distStr.c_str());
            ImGui::GetBackgroundDrawList()->AddText(ImVec2(head2D.X - distWidth / 2.f, head2D.Y - 30.f), ImColor(255, 255, 255, 255), distStr.c_str());
        }
    }

    inline void DrawWatermark() {
        char buffer[256]; snprintf(buffer, sizeof(buffer), "Build: %s (%s) | FPS: %.1f", globals.build_id.c_str(), globals.current_platform.c_str(), ImGui::GetIO().Framerate);
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), ImColor(255, 255, 255, 200), buffer);
    }

    inline void Draw() {
        DrawWatermark();
        if (config::g_drawCrosshair) global::render->DrawCrosshair(ImColor(config::g_colorCrosshair[0], config::g_colorCrosshair[1], config::g_colorCrosshair[2]));
        LocalPlayer lp = global::m_cheat->GetLocalPlayer();
        if (lp.FMinimalViewInfo.FOV < 5.0f || lp.FMinimalViewInfo.FOV > 170.0f) return;
        std::lock_guard<std::mutex> lock(g_state_mutex);
        for (auto& actor : global::m_cheat->actors) {
            ImColor color; bool shouldDraw = false, drawBox = false;
            switch (actor.type) {
            case EntityType::Survivor: shouldDraw = config::g_espSurvivors; color = ImColor(config::g_colorSurvivor[0], config::g_colorSurvivor[1], config::g_colorSurvivor[2]); drawBox = config::g_drawEspCornerBox; break;
            case EntityType::Killer: shouldDraw = config::g_espKillers; color = ImColor(config::g_colorKiller[0], config::g_colorKiller[1], config::g_colorKiller[2]); drawBox = config::g_drawEspCornerBox; break;
            case EntityType::Generator: shouldDraw = config::g_espGenerators; color = ImColor(config::g_colorGenerator[0], config::g_colorGenerator[1], config::g_colorGenerator[2]); break;
            }
            if (!shouldDraw) continue;
            if (actor.type == EntityType::Survivor || actor.type == EntityType::Killer) if (lp.FMinimalViewInfo.Location.Distance(actor.position) / 100.0 <= 2.0) continue;
            if (drawBox) Draw2DBox(actor.position, lp.FMinimalViewInfo, color);
            if (config::g_drawLines) DrawLines(actor.position, lp.FMinimalViewInfo, color);
            DrawNameAndDistance(actor.position, lp.FMinimalViewInfo, actor.Name, color, actor.type);
        }
    }
}