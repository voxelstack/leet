#pragma once

#include <stdio.h>
#include <windows.h>

#include <perflib.h>

#define _RUNS 100

#define setup()                                                               \
        LARGE_INTEGER _t0;                                                    \
        LARGE_INTEGER _t1;                                                    \
        LARGE_INTEGER _f;                                                     \
        long long _ts[_RUNS];

#define start()                                                               \
        QueryPerformanceFrequency(&_f);                                       \
                                                                              \
        _Pragma("GCC diagnostic push");                                       \
        _Pragma("GCC diagnostic ignored \"-Wimplicit-function-declaration\"")

#define benchmark(f)                                                          \
        printf("%s,", #f);                                                    \
        f();                                                                  \
        printf("%lld", _ts[0]);                                               \
        for (int _i = 1; _i < _RUNS; ++_i)                                    \
                printf(";%lld", _ts[_i]);                                     \
        printf("\n");

#define time_start()                                                          \
        for (int _i = 0; _i < _RUNS; ++_i)                                    \
        {                                                                     \
                QueryPerformanceCounter(&_t0);

#define time_end()                                                            \
        QueryPerformanceCounter(&_t1);                                        \
        _ts[_i] = (long long)((_t1.QuadPart - _t0.QuadPart) * 1000.0          \
                              / _f.QuadPart);                                 \
        }

#define end()                                                                 \
        _Pragma("GCC diagnostic pop");                                        \
                                                                              \
        return 0;
