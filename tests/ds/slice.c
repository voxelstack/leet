#include "../tests.h"

#include <ds/slice.h>

int
main()
{
        start();

        test(make);
        test(empty);
        test(resize);
        test(append);
        test(sappend);
        test(insert);
        test(sinsert);
        test(rwd);
        test(clear);
        test(at);

        end();
}

int
make()
{
        size_t el_size = sizeof(char);
        size_t el_no = 2;
        struct slice s = { 0 };

        slice_make(&s, el_size, el_no);

        should(eq(s._capacity, el_size * el_no),
               "capacity was not initialized");
        should(eq(s._el_size, el_size), "el_size was not initialized");
        should(eq(s._ptr, 0), "ptr was not initialized");
        should(!eq(s._data, NULL), "data was null");

        slice_del(&s);
        return 0;
}

int
empty()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice s;
        char el = 'c';

        slice_make(&s, el_size, el_no);
        should(slice_empty(&s), "empty was false for empty slice");

        slice_append(&s, &el);
        should(!slice_empty(&s), "empty was true for non-empty slice");

        slice_del(&s);
        return 0;
}

int
resize()
{
        size_t el_size = sizeof(char);
        size_t el_no = 2;
        struct slice s = { 0 };

        slice_make(&s, el_size, el_no);
        slice_resize(&s);

        should(eq(s._capacity, el_size * el_no * _SLICE_SCALE_FACTOR),
               "capacity was not scaled by scale factor");

        slice_del(&s);
        return 0;
}

int
append()
{
        size_t el_size = sizeof(char);
        size_t el_no = 2;
        struct slice s = { 0 };
        char el = 'c';

        slice_make(&s, el_size, el_no);
        slice_append(&s, &el);

        should(eq(*s._data, el), "element was not pushed");
        should(eq(s._ptr, el_size), "ptr was not advanced");

        slice_del(&s);
        return 0;
}

int
sappend()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice s = { 0 };
        char el = 'c';

        slice_make(&s, el_size, el_no);
        slice_sappend(&s, &el);

        should(eq(*s._data, el), "element was not pushed");
        should(eq(s._ptr, el_size), "ptr was not advanced");

        slice_sappend(&s, &el);
        should(eq(s._capacity, el_no * _SLICE_SCALE_FACTOR),
               "slice was not resized");

        slice_del(&s);
        return 0;
}

int
insert()
{
        size_t el_size = sizeof(int);
        size_t el_no = 5;
        struct slice s = { 0 };
        int el = 0;
        int res[] = { 1, 2, 0, 3, 4 };

        slice_make(&s, el_size, el_no);
        for (int i = 1; i <= 4; ++i)
                slice_append(&s, &i);
        slice_insert(&s, &el, 2);

        for (int i = 0; i < 5; ++i)
                should(eq(*(int*)slice_at(&s, i), res[i]),
                       "element order was incorrect");
        should(eq(s._ptr, 5 * el_size), "ptr was not advanced");

        slice_del(&s);
        return 0;
}

int
sinsert()
{
        size_t el_size = sizeof(int);
        size_t el_no = 4;
        struct slice s = { 0 };
        int el = 0;
        int res[] = { 1, 2, 0, 3, 4 };

        slice_make(&s, el_size, el_no);
        for (int i = 1; i <= 4; ++i)
                slice_append(&s, &i);
        slice_sinsert(&s, &el, 2);

        for (int i = 0; i < 5; ++i)
                should(eq(*(int*)slice_at(&s, i), res[i]),
                       "element order was incorrect");
        should(eq(s._ptr, 5 * el_size), "ptr was not advanced");
        should(eq(s._capacity, el_no * el_size * _SLICE_SCALE_FACTOR),
               "slice was not resized");

        slice_del(&s);
        return 0;
}

int
rwd()
{
        size_t el_size = sizeof(char);
        size_t el_no = 2;
        struct slice s = { 0 };
        char el = 'c';

        slice_make(&s, el_size, el_no);
        slice_append(&s, &el);
        slice_append(&s, &el);

        slice_rwd(&s, 2);
        should(eq(s._ptr, 0), "ptr was not rewinded");

        slice_del(&s);
        return 0;
}

int
clear()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice s = { 0 };
        char el = 'c';

        slice_make(&s, el_size, el_no);
        slice_append(&s, &el);

        slice_clear(&s);
        should(eq(s._ptr, 0), "ptr was not rewinded");

        slice_del(&s);
        return 0;
}

int
at()
{
        size_t el_size = sizeof(char);
        size_t el_no = 2;
        struct slice s = { 0 };
        char el0 = 'c';
        char el1 = 'd';

        slice_make(&s, el_size, el_no);
        slice_append(&s, &el0);
        slice_append(&s, &el1);

        char* dst;

        dst = slice_at(&s, 0);
        should(eq(*dst, el0), "returned value was not element at index");

        dst = slice_at(&s, 0);
        should(eq(*dst, el0), "returned value was not element at index");

        slice_del(&s);
        return 0;
}
