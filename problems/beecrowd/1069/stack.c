#pragma icanc include
#include <ds/arrstack.h>
#pragma icanc end

int
main()
{
        unsigned int no;
        char buf[1001];

        struct slice s = { 0 };
        arrstack_make(&s, sizeof(char), 1000);

        scanf("%u\n", &no);
        do
        {
                unsigned int res = 0;

                gets(buf);
                for (int i = 0; buf[i] != '\0'; ++i)
                {
                        if (buf[i] == '<')
                                arrstack_push(&s, buf + i);
                        else if (buf[i] == '>' && !arrstack_empty(&s))
                        {
                                arrstack_rwd(&s);
                                ++res;
                        }
                }
                printf("%u\n", res);
                arrstack_clear(&s);
        } while (--no);
        return 0;
}
