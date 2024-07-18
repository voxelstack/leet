#pragma icanc include
#include <leet.h>
#pragma icanc end

int
main()
{
        unsigned int no;
        char buf[1001];

        scanf("%u\n", &no);
        for (int n = 0; n < no; ++n)
        {
                unsigned int openno = 0;
                unsigned int res = 0;

                gets(buf);
                for (int i = 0; buf[i] != '\0'; ++i)
                {
                        if (buf[i] == '<')
                                ++openno;
                        else if (buf[i] == '>' && openno)
                        {
                                ++res;
                                --openno;
                        }
                }
                printf("%u\n", res);
        }
        return 0;
}
