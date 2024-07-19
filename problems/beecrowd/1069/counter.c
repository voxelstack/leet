#pragma icanc include
#include <leet.h>
#pragma icanc end

int
main()
{
        unsigned int no;
        char buf[1001];

        scanf("%u\n", &no);
        do
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
        } while (--no);
        return 0;
}
