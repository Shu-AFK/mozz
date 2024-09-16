#include "debug.h"

std::queue<std::string> writes;
int stdout_dupfd = -1;
bool should_stop = false;

void initConsole() {
    if (AllocConsole()) {
        SetConsoleTitle(reinterpret_cast<LPCSTR>("Debug Console"));
        stdout_dupfd = _dup(1);
        FILE* new_stdout;
        freopen_s(&new_stdout, "CONOUT$", "w", stdout);
    }

    while (!should_stop) {
        if (!writes.empty()) {
            std::cout << writes.front() << std::endl;
            writes.pop();
        }

        Sleep(500);
    }
}

void RunConsole() {
    std::thread console(initConsole);
    console.detach();
}

void StopConsole() {
    should_stop = true;
}

void EnqueConsoleWrite(const std::string &cmd) {
    writes.push(cmd);
}