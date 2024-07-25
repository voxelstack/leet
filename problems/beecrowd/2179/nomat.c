#pragma icanc include
#include <leet.h>
#pragma icanc end

int
main()
{
        int n;
        int r; // f
        int c;

        scanf("%i %i %i", &n, &r, &c);
        int nn = n * n;

        //           R  D  L  U
        int mv[] = { 1, n, -1, -n };
        int mvr[] = { 0, 1, 0, -1 };
        int mvc[] = { 1, 0, -1, 0 };
        size_t dir = 0;

        int cell = n * r + c + 1;
        int steps = 1;
        int visited = nn - 1;
        int total_visited = 1;

        printf("%u", cell);
        while (1)
        {
                int two = 2;
                do
                {
                        int curr_steps = steps;
                        do
                        {
                                cell += mv[dir];
                                r += mvr[dir];
                                c += mvc[dir];

                                ++total_visited;
                                if (r >= 0 && c >= 0 && r < n && c < n)
                                {
                                        printf(" %i", cell);
                                        if (!--visited)
                                        {
                                                printf("\n%i\n",
                                                       total_visited);
                                                return 0;
                                        }
                                }
                        } while (--curr_steps);
                        dir = (dir + 1) % 4;
                } while (--two);
                ++steps;
        };
        return 0;
}
