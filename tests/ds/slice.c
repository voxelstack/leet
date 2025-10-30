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
        test(delete_at);
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
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;

        should(eq(h->capacity, el_size * el_no),
               "capacity was not initialized");
        should(eq(h->el_size, el_size), "el_size was not initialized");
        should(eq(h->len, 0), "len was not initialized");
        should(!eq(h->data, NULL), "data was not allocated");

        slice_del(s);
        return 0;
}

int
empty()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;

        should(slice_empty(s), "empty was false for empty slice");

        char el = 'c';
        slice_append(s, &el);

        should(!slice_empty(s), "empty was true for non-empty slice");

        slice_del(s);
        return 0;
}

int
resize()
{
        size_t el_size = sizeof(char);
        size_t el_no = 2;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;
        slice_resize(s);

        should(eq(h->capacity, el_size * el_no * _SLICE_SCALE_FACTOR),
               "capacity was not scaled by scale factor");

        slice_del(s);
        return 0;
}

int
append()
{
        size_t el_size = sizeof(char);
        size_t el_no = 2;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;

        char el = 'c';
        slice_append(s, &el);

        should(eq(*h->data, el), "element was not pushed");
        should(eq(h->len, 1), "len was not updated");

        slice_del(s);
        return 0;
}

int
sappend()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;

        char el = 'c';
        slice_sappend(s, &el);

        should(eq(*h->data, el), "element was not pushed");
        should(eq(h->len, 1), "len was not updated");

        slice_sappend(s, &el);
        should(eq(h->capacity, el_no * _SLICE_SCALE_FACTOR),
               "slice was not resized");

        slice_del(s);
        return 0;
}

int
insert()
{
        size_t el_size = sizeof(int);
        size_t el_no = 5;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;
        int el = 0;
        int res[] = { 1, 2, 0, 3, 4 };

        for (int i = 1; i <= 4; ++i)
                slice_append(s, &i);
        slice_insert(s, &el, 2);

        for (int i = 0; i < 5; ++i)
                should(eq(*(int*)slice_at(s, i), res[i]),
                       "element order was incorrect");
        should(eq(h->len, 5), "len was not updated");

        slice_del(s);
        return 0;
}

int
sinsert()
{
        size_t el_size = sizeof(int);
        size_t el_no = 4;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;
        int el = 0;
        int res[] = { 1, 2, 0, 3, 4 };

        for (int i = 1; i <= 4; ++i)
                slice_append(s, &i);
        slice_sinsert(s, &el, 2);

        for (int i = 0; i < 5; ++i)
                should(eq(*(int*)slice_at(s, i), res[i]),
                       "element order was incorrect");
        should(eq(h->len, 5), "len was not updated");
        should(eq(h->capacity, el_no * el_size * _SLICE_SCALE_FACTOR),
               "slice was not resized");

        slice_del(s);
        return 0;
}

int
delete_at()
{
        size_t el_size = sizeof(int);
        size_t el_no = 5;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;
        int el = 0;
        int res[] = { 0, 1, 3, 4 };

        for (int i = 0; i < 5; ++i)
        {
                slice_append(s, &i);
        }

        slice_delete_at(s, 2);

        for (int i = 0; i < 4; ++i)
        {
                should(eq(*(int*)slice_at(s, i), res[i]),
                       "element order was incorrect");
        }
        should(eq(h->len, 4), "len was not updated");

        slice_del(s);
        return 0;
}

int
rwd()
{
        size_t el_size = sizeof(char);
        size_t el_no = 2;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;
        char el = 'c';

        slice_append(s, &el);
        slice_append(s, &el);

        slice_rwd(s, 2);
        should(eq(h->len, 0), "len was not updated");

        slice_del(s);
        return 0;
}

int
clear()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;
        char el = 'c';

        slice_append(s, &el);

        slice_clear(s);
        should(eq(h->len, 0), "len was not updated");

        slice_del(s);
        return 0;
}

int
at()
{
        size_t el_size = sizeof(char);
        size_t el_no = 2;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;
        char el0 = 'c';
        char el1 = 'd';

        slice_append(s, &el0);
        slice_append(s, &el1);

        char* dst;

        dst = slice_at(s, 0);
        should(eq(*dst, el0), "returned value was not element at index");

        dst = slice_at(s, 0);
        should(eq(*dst, el0), "returned value was not element at index");

        slice_del(s);
        return 0;
}
