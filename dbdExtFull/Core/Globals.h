#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define IMGUI_DEFINE_MATH_OPERATORS

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <mutex> 
#include <random>
#include <atomic>
#include <Windows.h>
#include <D3D11.h>

#ifndef _D3DMATRIX_DEFINED
#define _D3DMATRIX_DEFINED
typedef struct _D3DMATRIX {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
} D3DMATRIX;
#endif

namespace SDK {
    struct FVector {
        double X, Y, Z;
        FVector() : X(0), Y(0), Z(0) {}
        FVector(double x, double y, double z) : X(x), Y(y), Z(z) {}
        double Dot(const FVector& other) const { return X * other.X + Y * other.Y + Z * other.Z; }
        FVector operator-(const FVector& other) const { return FVector(X - other.X, Y - other.Y, Z - other.Z); }
        double Distance(const FVector& v) const {
            return std::sqrt(std::pow(X - v.X, 2) + std::pow(Y - v.Y, 2) + std::pow(Z - v.Z, 2));
        }
    };

    struct FRotator { double Pitch, Yaw, Roll; };
    struct FMinimalViewInfo { FVector Location; FRotator Rotation; float FOV; };
    struct FCameraCacheEntry { FMinimalViewInfo POV; };
}

enum class EntityType {
    Survivor, Killer, Generator, Pallet, Window, Hook, Unknown
};

struct Entity {
    SDK::FVector position;
    std::string Name;
    EntityType type;
};

struct LocalPlayer {
    SDK::FMinimalViewInfo FMinimalViewInfo;
};

namespace config {
    inline bool g_espSurvivors = true;
    inline float g_colorSurvivor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    inline bool g_espKillers = true;
    inline float g_colorKiller[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    inline bool g_espGenerators = true;
    inline float g_colorGenerator[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
    inline bool g_drawEspCornerBox = true;
    inline bool g_drawLines = false;
    inline bool g_drawDistance = true;
    inline bool g_drawName = true;
    inline float g_colorName[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline bool g_drawCrosshair = true;
    inline float g_colorCrosshair[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    inline bool g_auraEsp = false;
    inline bool g_fovChanger = false;
    inline float g_fovValue = 100.0f;
    inline bool g_aspectRatioChanger = false;
    inline float g_aspectRatioValue = 1.777f;
    inline bool g_autoSkillcheck = true;
    inline float g_skillcheckBuffer = 0.015f;
    inline bool g_infiniteLunge = false;
    inline bool g_noCooldown = false;
    inline bool g_instantBell = false;
    inline bool g_speedHack = false;
    inline float g_speedValue = 1.05f;
    inline bool g_autoDeadHard = false;
    inline int  g_autoDeadHardKey = VK_XBUTTON2;
    inline int  g_deadHardKey = 'E';
}

struct Globals {
    int ScreenWidth = 1920;
    int ScreenHeight = 1080;
    HWND window_handle = NULL;
    HWND game_hwnd = NULL;
    uintptr_t base_address = 0;
    std::string current_platform = "Unknown";
    bool show_menu = false;
    bool vsync = true;
    bool should_exit = false;
    float ScreenCenterX = 960.0f;
    float ScreenCenterY = 540.0f;
    std::string rand_class_name;
    std::string rand_window_name;
    std::string build_id;
    std::atomic<uintptr_t> killer_pawn{ 0 };
};
inline Globals globals;
inline std::mutex g_state_mutex;