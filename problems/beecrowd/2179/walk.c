#pragma icanc include
#include <ds/mat.h>
#include <leet.h>
#pragma icanc end

int
main()
{
        int n;
        int r; // f
        int c;
        struct mat m = { 0 };

        scanf("%i %i %i", &n, &r, &c);
        int nn = n * n;

        mat_make(&m, sizeof(int), n, n);
        for (int i = 0; i < nn;)
                ((int*)m._data)[i++] = i;

        //            R  D  L  U
        int mvr[] = { 0, 1, 0, -1 };
        int mvc[] = { 1, 0, -1, 0 };
        size_t dir = 0;

        int steps = 1;
        int visited = nn - 1;
        int total_visited = 1;

        printf("%i", *(int*)mat_at(&m, r, c));
        while (1)
        {
                int two = 2;
                do
                {
                        int curr_steps = steps;
                        do
                        {
                                r += mvr[dir];
                                c += mvc[dir];

                                ++total_visited;
                                if (r >= 0 && c >= 0 && r < n && c < n)
                                {
                                        printf(" %i", *(int*)mat_at(&m, r, c));
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

        mat_del(&m);
        return 0;
}
