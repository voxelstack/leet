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
        struct slice s = { 0 };
        char el = 'c';

        arrstack_make(&s, el_size, el_no);
        arrstack_push(&s, &el);

        char* dst = arrstack_peek(&s);
        should(eq(*dst, el), "returned value was not the top of the stack");
        should(eq(s._ptr, el_size), "ptr was changed");

        return 0;
}

int
pop()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice s = { 0 };
        char el = 'c';
        char dst;

        arrstack_make(&s, el_size, el_no);
        arrstack_push(&s, &el);

        arrstack_pop(&s, &dst);
        should(eq(dst, el), "returned value was not the top of the stack");
        should(eq(s._ptr, 0), "ptr was not rewinded");

        return 0;
}

int
rwd()
{
        size_t el_size = sizeof(char);
        size_t el_no = 1;
        struct slice s = { 0 };
        char el = 'c';
        char dst;

        arrstack_make(&s, el_size, el_no);
        arrstack_push(&s, &el);

        arrstack_rwd(&s);
        should(eq(s._ptr, 0), "ptr was not rewinded");

        return 0;
}
