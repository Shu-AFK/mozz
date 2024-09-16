// TODO: Continue implementation: https://github.com/hasherezade/process_doppelganging/blob/master/main.cpp

#include "process_doppeling.h"

HANDLE MakeTransectedSection(BYTE *buf, DWORD size) {
    DWORD options, isolationLvl, isolationFlags, timeout;
    options = isolationLvl = isolationFlags = timeout = 0;

    HANDLE hTransaction = CreateTransaction(nullptr, nullptr, options, isolationLvl, isolationFlags, timeout, nullptr);
    if (hTransaction == INVALID_HANDLE_VALUE) {

        return INVALID_HANDLE_VALUE;
    }

    return nullptr;
}

bool ProcessDoppel(wchar_t *targetPath, BYTE *payloadBuf, DWORD payloadSize) {
    HANDLE hSection = MakeTransectedSection(payloadBuf, payloadSize);
    if (!hSection || hSection == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}