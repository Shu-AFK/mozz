#ifndef MOZZ_PRINT_H
#define MOZZ_PRINT_H

#include "include.h"

enum PrintLevel {
    PRINT_DEBUG = 0,
    PRINT_ERROR = 1,
    PRINT_INFO = 2,
};

#ifdef DEBUG
extern const std::vector<std::string> PrintSigns;

#define PRINT(str, level) \
    EnqueConsoleWrite(std::format("{} {}", PrintSigns[level], str))
#else
#define PRINT(str, level)
#endif

#endif //MOZZ_PRINT_H
