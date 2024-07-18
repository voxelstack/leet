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

        return 0;
}
