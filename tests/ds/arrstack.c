#include "../tests.h"

#include <ds/arrstack.h>

int
main()
{
        start();

        test(peek);
        test(pop);
        test(rwd);

        end();
}

int
peek()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;
        char el = 'c';

        arrstack_push(s, &el);

        char* dst = arrstack_peek(s);
        should(eq(*dst, el), "returned value was not the top of the stack");
        should(eq(s->len, 1), "ptr was changed");

        arrstack_del(s);
        return 0;
}

int
pop()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;
        char el = 'c';
        char dst;

        arrstack_push(s, &el);

        arrstack_pop(s, &dst);
        should(eq(dst, el), "returned value was not the top of the stack");
        should(eq(s->len, 0), "len was not updated");

        arrstack_del(s);
        return 0;
}

int
rwd()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice* s = slice_make(el_size, el_no);
        struct _slice* h = (struct _slice*)s;
        char el = 'c';
        char dst;

        arrstack_push(s, &el);

        arrstack_rwd(s);
        should(eq(s->len, 0), "len was not updated");

        arrstack_del(s);
        return 0;
}
