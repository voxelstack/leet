#include <leet.h>
#include <ds/arrstack.h>

void
help()
{
        printf("help: h\n");
        printf("exit: .\n");
        printf("push: < 123\n");
        printf("pop : >\n");
        printf("op  : +,-,*,/\n");
}

void
bin_op(struct slice* s, char op)
{
        if (s->len < 2)
        {
                printf("err\n");
                return;
        }

        int r = *(int*)slice_at(s, s->len - 1);
        int l = *(int*)slice_at(s, s->len - 2);
        int res;

        switch (op)
        {
        case '+':
                res = r + l;
                break;
        case '-':
                res = r - l;
                break;
        case '*':
                res = r * l;
                break;
        case '/':
                res = r / l;
                break;
        }

        slice_rwd(s, 2);
        arrstack_push(s, &res);
}

int
main()
{
        char in[32];
        char op;
        int val;

        struct slice* s = arrstack_make(sizeof(int), 128);

        printf("| h\n");
        help();

        do
        {
                printf("| ");

                fgets(in, sizeof(in), stdin);
                if (sscanf(in, "%c %d\n", &op, &val) == 1)
                {
                        switch (op)
                        {
                        case '.':
                                printf("end\n");
                                return 0;
                        case 'h':
                                help();
                                break;
                        case '>':
                                if (arrstack_empty(s))
                                {
                                        printf("nil\n");
                                        continue;
                                }
                                int* p = arrstack_peek(s);
                                printf("%d\n", *p);
                                arrstack_rwd(s);
                                break;
                        case '+':
                        case '-':
                        case '*':
                        case '/':
                                bin_op(s, op);
                                break;
                        default:
                                printf("err\n");
                                continue;
                        }
                }
                else
                {
                        switch (op)
                        {
                        case '<':
                                if (slice_full(s))
                                {
                                        printf("full\n");
                                        continue;
                                }
                                arrstack_push(s, &val);
                                break;
                        default:
                                printf("err\n");
                                continue;
                        }
                }
        } while (true);

        return 0;
}
