#include "../tests.h"

#include <alg/sort.h>
#include <ds/slice.h>

int
main()
{
        start();

        test(merge);

        end();
}

// TODO Better way of reusing these snippets.

#define make_arr()                                                            \
        /* chosen by fair dice roll. */                                       \
        /* guaranteed to be random. */                                        \
        int a[] = { 11, 8,  9,  18, 17, 12, 16, 4, 5, 14,                     \
                    19, 13, 10, 6,  3,  20, 15, 2, 1, 7 };                    \
        size_t arrno = sizeof(a) / sizeof(int);                               \
        struct _slice _arr = { .data = (byte*)a,                              \
                               .len = arrno,                                  \
                               .capacity = sizeof(a),                         \
                               .el_size = sizeof(int) };                      \
        struct slice* arr = (struct slice*)&_arr

#define should_be_sorted()                                                    \
        do                                                                    \
        {                                                                     \
                should(eq(arrno, *(int*)slice_at(arr, arrno - 1)),            \
                       "array was not sorted");                               \
        } while (--arrno);

int
comparator(void* a, void* b)
{
        return *(int*)a - *(int*)b;
}

int
merge()
{
        make_arr();
        struct slice* w = slice_make(sizeof(int), arrno);

        sort_merge(arr, w, comparator, 0, arrno - 1);

        should_be_sorted();

        slice_del(w);
        return 0;
}
