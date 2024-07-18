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
        for (int n = 0; n < no; ++n)
        {
                unsigned int res = 0;
                arrstack_clear(&s);

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
        }
        return 0;
}
