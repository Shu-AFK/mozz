#include "print.h"

#ifdef DEBUG
#include "debug.h"

std::vector<std::string> PrintSigns = {
        "[*]",
        "[-]",
        "[+]",
};
#endif

void Print(const std::string &str, PrintLevel level) {
#ifdef DEBUG
    EnqueConsoleWrite(std::format("{} {}", PrintSigns[level], str));
#endif
}