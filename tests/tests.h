#pragma once

#include <stdio.h>

#define start()                                                               \
        int _failures = 0;                                                    \
        printf("%s\n", __FILE__);                                             \
                                                                              \
        _Pragma("GCC diagnostic push");                                       \
        _Pragma("GCC diagnostic ignored \"-Wimplicit-function-declaration\"")

#define test(f)                                                               \
        printf("  %s\n", #f);                                                 \
        _failures += f();

#define end()                                                                 \
        _Pragma("GCC diagnostic pop");                                        \
                                                                              \
        printf("%s\n", _failures ? "FAIL" : "PASS");                          \
        return _failures;

#define should(c, msg)                                                        \
        if (!(c))                                                             \
        {                                                                     \
                printf("    %s\n", msg);                                      \
                return 1;                                                     \
        }

#define fail(msg) should(false, msg);

#define eq(a, b) ((a) == (b))
