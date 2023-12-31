#include "panic.h"

void panic(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    vprintf("Panic: ", args);
    vprintf(msg, args);
    vprintf("\n", args);
    va_end(args);
    exit(1);
}
