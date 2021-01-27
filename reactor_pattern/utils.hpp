#pragma once
#include <cstring>
#include <cstdio>
#include <errno.h>    //for error handling
#include <cstdarg>


namespace happyangel {

/* error - print a diagnostic and optionally exit */
void error(int status, int err, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (err)
        fprintf(stderr, ": %s (%d)\n", strerror(err), err);
    if (status)
        exit(status);
}

}