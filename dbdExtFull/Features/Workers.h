#pragma once
#include "../Core/Driver.h"
#include "../Core/Renderer.h"
#include "Exploits.h"

inline void RunEntityUpdateLoop() {
    while (!globals.should_exit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        if (!globals.base_address) continue;
        uintptr_t uworld = Kernel->read<uintptr_t>(globals.base_address + Offsets::GWorld); if (!valid_ptr(uworld)) continue;
        uintptr_t game_instance = Kernel->read<uintptr_t>(uworld + Offsets::OwningGameInstance);
        uintptr_t local_players_array = Kernel->read<uintptr_t>(game_instance + Offsets::LocalPlayers);
        uintptr_t local_player = Kernel->read<uintptr_t>(local_players_array);
        uintptr_t player_controller = Kernel->read<uintptr_t>(local_player + Offsets::PlayerController);
        uintptr_t local_pawn = Kernel->read<uintptr_t>(player_controller + Offsets::AcknowledgedPawn);
        uintptr_t level = Kernel->read<uintptr_t>(uworld + Offsets::PersistentLevel); if (!valid_ptr(level)) continue;
        uintptr_t actor_array = Kernel->read<uintptr_t>(level + Offsets::ActorArray);
        int actor_count = Kernel->read<int>(level + Offsets::ActorCount);
        if (!valid_ptr(actor_array) || actor_count <= 0 || actor_count > 15000) continue;
        std::vector<Entity> temp_list;
        for (int i = 0; i < actor_count; ++i) {
            uintptr_t actor = Kernel->read<uintptr_t>(actor_array + (i * 0x8)); if (!valid_ptr(actor) || actor == local_pawn) continue;
            EntityType type = EntityType::Unknown; std::string displayName;
            uintptr_t pState = Kernel->read<uintptr_t>(actor + Offsets::PlayerState);
            if (valid_ptr(pState)) {
                uint8_t gameRole = Kernel->read<uint8_t>(pState + Offsets::GameRole);
                if (gameRole == 1) { type = EntityType::Killer; displayName = "Killer"; globals.killer_pawn = actor; }
                else if (gameRole == 2) { type = EntityType::Survivor; displayName = "Survivor"; }
            }
            if (type == EntityType::Unknown) {
                uintptr_t genComp = Kernel->read<uintptr_t>(actor + Offsets::GeneratorChargeable);
                if (valid_ptr(genComp)) { type = EntityType::Generator; displayName = "Generator"; }
            }
            if (type == EntityType::Survivor || type == EntityType::Killer) {
                uintptr_t outline_comp = Kernel->read<uintptr_t>(actor + (type == EntityType::Survivor ? Offsets::OutlineComponentSurvivor : Offsets::OutlineComponentKiller));
                if (valid_ptr(outline_comp)) Kernel->write<float>(outline_comp + Offsets::AuraInterpolationSpeed, config::g_auraEsp ? 1.0f : 0.0f);
            }
            if (type == EntityType::Unknown) continue;
            uintptr_t root = Kernel->read<uintptr_t>(actor + Offsets::RootComponent); if (!valid_ptr(root)) continue;
            Entity ent = {}; ent.position = Kernel->read<SDK::FVector>(root + Offsets::RelativeLocation); ent.Name = displayName; ent.type = type;
            temp_list.push_back(ent);
        }
        std::lock_guard<std::mutex> lock(g_state_mutex);
        global::m_cheat->actors = temp_list;
    }
}

inline void RunLocalPlayerWorker() {
    bool has_pressed_this_check = false;
    while (!globals.should_exit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (!globals.base_address) continue;
        uintptr_t uworld = Kernel->read<uintptr_t>(globals.base_address + Offsets::GWorld); if (!valid_ptr(uworld)) continue;
        uintptr_t game_instance = Kernel->read<uintptr_t>(uworld + Offsets::OwningGameInstance); if (!valid_ptr(game_instance)) continue;
        uintptr_t local_players_array = Kernel->read<uintptr_t>(game_instance + Offsets::LocalPlayers); if (!valid_ptr(local_players_array)) continue;
        uintptr_t local_player = Kernel->read<uintptr_t>(local_players_array); if (!valid_ptr(local_player)) continue;
        uintptr_t player_controller = Kernel->read<uintptr_t>(local_player + Offsets::PlayerController); if (!valid_ptr(player_controller)) continue;
        uintptr_t pawn = Kernel->read<uintptr_t>(player_controller + Offsets::AcknowledgedPawn); if (!valid_ptr(pawn)) continue;
        tick_attack_exploits(pawn); tick_wraith_exploits(pawn); tick_speed_hack(pawn); tick_deadhard(pawn, globals.killer_pawn.load());
        if (config::g_fovChanger || config::g_aspectRatioChanger) {
            uintptr_t camera_comp = Kernel->read<uintptr_t>(pawn + Offsets::CameraComponent);
            if (valid_ptr(camera_comp)) {
                if (config::g_fovChanger) Kernel->write<float>(camera_comp + Offsets::CameraFOV, config::g_fovValue);
                if (config::g_aspectRatioChanger) Kernel->write<float>(camera_comp + Offsets::CameraAspectRatio, config::g_aspectRatioValue);
            }
        }
        if (config::g_autoSkillcheck) {
            uintptr_t interaction_handler = Kernel->read<uintptr_t>(pawn + Offsets::InteractionHandler);
            if (valid_ptr(interaction_handler)) {
                uintptr_t skill_check = Kernel->read<uintptr_t>(interaction_handler + Offsets::SkillCheck);
                if (valid_ptr(skill_check)) {
                    bool is_displayed = Kernel->read<bool>(skill_check + Offsets::SkillCheck_IsDisplayed);
                    if (is_displayed) {
                        float progress = Kernel->read<float>(skill_check + Offsets::SkillCheck_CurrentProgress);
                        uintptr_t def_addr = skill_check + Offsets::SkillCheck_Definition;
                        float success_start = Kernel->read<float>(def_addr + 0x00), bonus_start = Kernel->read<float>(def_addr + 0x08);
                        float bonus_len = Kernel->read<float>(def_addr + 0x0C);
                        float target_zone = (bonus_len > 0.0f) ? bonus_start : success_start;
                        if (target_zone > 0.05f) {
                            float hit_target = target_zone - config::g_skillcheckBuffer;
                            if (progress < 0.05f) has_pressed_this_check = false;
                            if (!has_pressed_this_check && progress >= hit_target && progress < hit_target + 0.15f) {
                                std::thread([]() { SendHardwareKey(VK_SPACE); }).detach(); has_pressed_this_check = true;
                            }
                        }
                    } else has_pressed_this_check = false;
                }
            }
        }
    }
}