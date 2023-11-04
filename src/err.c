#include "include/err.h"

void error(size_t line, size_t pos, char *msg) { report(line, pos, "", msg); }

void report(size_t line, size_t pos, char *where, char *msg) {
    fprintf(stderr, "[line %zu; pls %zu] Error %s: '%s'", line, pos, where,
            msg);
}
