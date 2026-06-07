#pragma once
#include <Windows.h>
#include <string>
#include <random>
#include <thread>
#include <chrono>

inline std::string GenerateRandomString(size_t length) {
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device random_device; std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);
    std::string random_string;
    for (std::size_t i = 0; i < length; ++i) random_string += characters[distribution(generator)];
    return random_string;
}

inline void SendHardwareKey(WORD vKey) {
    INPUT input = { 0 }; input.type = INPUT_KEYBOARD; input.ki.wVk = vKey;
    input.ki.wScan = MapVirtualKey(vKey, MAPVK_VK_TO_VSC); input.ki.dwFlags = KEYEVENTF_SCANCODE;
    SendInput(1, &input, sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}