#pragma once
#include "Globals.h"
#include "Offsets.h"
#include <winioctl.h>
#include <TlHelp32.h>
#include <winternl.h>
#include <memory>

#ifndef FILE_SPECIAL_ACCESS
#define FILE_SPECIAL_ACCESS FILE_ANY_ACCESS
#endif

#define DRIVER_DEVICE_NAME "\\\\.\\Nul" 
#define IOCTL_FETCH_ADDRESS   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_WRITE_MEMORY    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_READ_MEMORY     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _KERNEL_REQUEST {
    DWORD ProcessID; uintptr_t Address; void* Buffer; SIZE_T Size; DWORD ControlCode;
} KERNEL_REQUEST, * PKERNEL_REQUEST;

inline __int64 request(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock, std::uint32_t IoControlCode, PVOID InputBuffer,
    std::uint32_t InputBufferLength, PVOID OutputBuffer, std::uint32_t OutputBufferLength)
{
    static auto NtDeviceIoControlFile = reinterpret_cast<NTSTATUS(NTAPI*)(
        HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, ULONG, PVOID, ULONG, PVOID, ULONG
        )>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtDeviceIoControlFile"));
    if (!NtDeviceIoControlFile) return 0xC000007AL;
    return NtDeviceIoControlFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock,
        IoControlCode, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength);
}

inline bool valid_ptr(uintptr_t ptr) { return (ptr > 0x400000 && ptr < 0x7FFFFFFFFFFF); }

class kernel {
public:
    HANDLE g_driver_handle = INVALID_HANDLE_VALUE;
    INT32 g_process_id = 0;

    bool driverconnect() {
        g_driver_handle = CreateFileA(DRIVER_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
        return g_driver_handle != INVALID_HANDLE_VALUE;
    }

    INT32 get_process_id(const wchar_t* process_name) {
        PROCESSENTRY32W pt; HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pt.dwSize = sizeof(PROCESSENTRY32W);
        if (Process32FirstW(hsnap, &pt)) {
            do { if (!lstrcmpiW(pt.szExeFile, process_name)) { CloseHandle(hsnap); return pt.th32ProcessID; } } while (Process32NextW(hsnap, &pt));
        }
        CloseHandle(hsnap); return 0;
    }

    uintptr_t get_base_address() {
        IO_STATUS_BLOCK status_block = {}; uintptr_t image_address = 0; KERNEL_REQUEST req; ZeroMemory(&req, sizeof(req));
        req.ProcessID = g_process_id; req.Buffer = &image_address; req.Size = sizeof(uintptr_t);
        request(g_driver_handle, nullptr, nullptr, nullptr, &status_block, IOCTL_FETCH_ADDRESS, &req, sizeof(req), &req, sizeof(req));
        return image_address;
    }

    bool read_physical(PVOID address, PVOID buffer, DWORD size) {
        IO_STATUS_BLOCK status_block = {}; KERNEL_REQUEST req; ZeroMemory(&req, sizeof(req));
        req.ProcessID = g_process_id; req.Address = reinterpret_cast<uintptr_t>(address); req.Buffer = buffer; req.Size = size;
        return request(g_driver_handle, nullptr, nullptr, nullptr, &status_block, IOCTL_READ_MEMORY, &req, sizeof(req), &req, sizeof(req)) >= 0;
    }

    bool write_physical(PVOID address, PVOID buffer, DWORD size) {
        IO_STATUS_BLOCK status_block = {}; KERNEL_REQUEST req; ZeroMemory(&req, sizeof(req));
        req.ProcessID = g_process_id; req.Address = reinterpret_cast<uintptr_t>(address); req.Buffer = buffer; req.Size = size;
        return request(g_driver_handle, nullptr, nullptr, nullptr, &status_block, IOCTL_WRITE_MEMORY, &req, sizeof(req), &req, sizeof(req)) >= 0;
    }

    template <typename T> T read(uintptr_t address) {
        T buffer{}; if (valid_ptr(address)) read_physical(reinterpret_cast<PVOID>(address), &buffer, sizeof(T)); return buffer;
    }

    template <typename T> bool write(uintptr_t address, const T& value) {
        if (valid_ptr(address)) return write_physical(reinterpret_cast<PVOID>(address), const_cast<T*>(&value), sizeof(T)); return false;
    }
};
inline const auto Kernel = std::make_unique<kernel>();