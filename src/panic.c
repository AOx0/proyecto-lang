#include "panic.h"

void panic(const char * msg, ...) {
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    exit(1);
}
