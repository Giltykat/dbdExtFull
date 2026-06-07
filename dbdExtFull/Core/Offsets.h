#pragma once
#include <cstdint>

namespace Offsets {
    inline uint64_t GWorld = 0x0;
    constexpr uint64_t GWorld_Steam = 0xD2C6938;
    constexpr uint64_t GWorld_Epic = 0xC719568;
    constexpr uint64_t GWorld_MS = 0xC7A5618;

    constexpr uint64_t OwningGameInstance = 0x250;
    constexpr uint64_t LocalPlayers = 0x58;
    constexpr uint64_t PlayerController = 0x50;
    constexpr uint64_t AcknowledgedPawn = 0x3A0;
    constexpr uint64_t PlayerCameraManager = 0x3B0;
    constexpr uint64_t CameraCachePrivate = 0x15A0;

    constexpr uint64_t PlayerState = 0x310;
    constexpr uint64_t GameRole = 0x3DA;
    constexpr uint32_t CustomTimeDilation = 0x88;

    constexpr uint64_t RootComponent = 0x1E0;
    constexpr uint64_t RelativeLocation = 0x178;
    constexpr uint64_t PersistentLevel = 0x50;
    constexpr uint64_t ActorArray = 0xC0;
    constexpr uint64_t ActorCount = 0xC8;
    constexpr uint64_t ActorID = 0x18;
    constexpr uint64_t GeneratorChargeable = 0x508;

    constexpr uintptr_t InteractionHandler = 0xB60;
    constexpr uintptr_t SkillCheck = 0x358;
    constexpr uintptr_t SkillCheck_IsDisplayed = 0x1A8;
    constexpr uintptr_t SkillCheck_CurrentProgress = 0x1AC;
    constexpr uintptr_t SkillCheck_Definition = 0x208;

    constexpr uintptr_t AttackerComponent = 0x1A58;
    constexpr uintptr_t CurrentAttack = 0x190;
    constexpr uintptr_t AttackStates = 0x3E0;
    constexpr uintptr_t AttackDuration = 0x48;
    constexpr uintptr_t SlasherAttackState = 0xC18;

    constexpr uintptr_t Mesh = 0x370;
    constexpr uintptr_t AnimScriptInstance = 0x358;
    constexpr uintptr_t WraithCloakingPlayRate = 0x878;

    constexpr uintptr_t OutlineComponentSurvivor = 0x1CE0;
    constexpr uintptr_t OutlineComponentKiller = 0x1C98;
    constexpr uintptr_t AuraInterpolationSpeed = 0x0310;

    constexpr uintptr_t CameraComponent = 0x0A58;
    constexpr uintptr_t CameraFOV = 0x0280;
    constexpr uintptr_t CameraAspectRatio = 0x02A4;
}