#include "../benchmarks.h"

#include <ds/bstree.h>

setup();

int
main()
{
        start();

        benchmark(fnptr);
        benchmark(nofnptr);

        end();
}

struct holder
{
        int data;
        struct bstree bst;
};

void
insert_fnptr(struct bstree** root, struct bstree* n, int (*cmp)(void*, void*))
{
        struct bstree** link = root;
        struct bstree* parent = NULL;
        while (*link)
        {
                parent = *link;
                // Allowing duplicates will not affect the benchmark.
                if (cmp(n, *link) < 0)
                        link = &((*link)->_left);
                else
                        link = &((*link)->_right);
        }

        bstree_link(link, n, parent);
}

void
insert_nofnptr(struct bstree** root, struct holder* n)
{
        struct bstree** link = root;
        struct bstree* parent = NULL;
        while (*link)
        {
                struct holder* this = container_of(*link, struct holder, bst);
                int result = n->data - this->data;

                parent = *link;
                // Allowing duplicates will not affect the benchmark.
                if (result < 0)
                        link = &((*link)->_left);
                else
                        link = &((*link)->_right);
        }

        bstree_link(link, &n->bst, parent);
}

int
comparator(void* a, void* b)
{
        return container_of(a, struct holder, bst)->data
               - container_of(b, struct holder, bst)->data;
}

int
fnptr()
{
        int n = 1000000;
        struct holder* nodes = malloc(n * sizeof(struct holder));
        struct bstree* root;

        for (int i = 0; i < n; ++i)
                nodes[i].data = rand();

        time_start();
        root = NULL;
        for (int i = 0; i < n; ++i)
                insert_fnptr(&root, &nodes[i].bst, comparator);
        time_end();

        free(nodes);
        return 0;
}

int
nofnptr()
{
        int n = 1000000;
        struct holder* nodes = malloc(n * sizeof(struct holder));
        struct bstree* root;

        for (int i = 0; i < n; ++i)
                nodes[i].data = rand();

        time_start();
        root = NULL;
        for (int i = 0; i < n; ++i)
                insert_nofnptr(&root, &nodes[i]);
        time_end();

        free(nodes);
        return 0;
}
