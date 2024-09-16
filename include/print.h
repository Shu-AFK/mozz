#ifndef MOZZ_PRINT_H
#define MOZZ_PRINT_H

#include "include.h"

enum PrintLevel {
    PRINT_DEBUG = 0,
    PRINT_ERROR = 1,
    PRINT_INFO = 2,
};

void Print(const std::string &str, PrintLevel level);

#endif //MOZZ_PRINT_H
