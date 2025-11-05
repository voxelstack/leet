#include "../tests.h"

#include <ds/btree.h>

int
main()
{
        start();

        test(create);
        test(insert_delete);
        test(insert_delete_random);
        test(insert_random_delete);

        end();
}

int
create()
{
        size_t key_size = sizeof(char);
        size_t value_size = sizeof(int);
        struct btree* tree = btree_create(key_size, value_size);

        btree_destroy(tree);

        return 0;
}

int
cmp_int(void* a, void* b)
{
        return *(int*)a - *(int*)b;
}

int vals[]
    = { 44, 19, 13, 39, 94, 7,  36, 75, 77, 24, 52, 49, 28, 79, 88, 26, 59,
        12, 35, 33, 67, 78, 96, 71, 14, 41, 5,  53, 83, 66, 34, 60, 45, 40,
        98, 92, 27, 99, 69, 65, 74, 54, 1,  89, 61, 76, 57, 84, 80, 97, 46,
        64, 32, 29, 81, 87, 68, 42, 91, 93, 9,  2,  23, 37, 48, 58, 50, 73,
        43, 86, 72, 18, 56, 0,  38, 70, 85, 22, 63, 82, 47, 30, 55, 62, 90,
        16, 3,  31, 25, 21, 20, 17, 8,  51, 95, 15, 10, 4,  6,  11 };
int valno = sizeof(vals) / sizeof(int);

int
insert_delete()
{
        struct btree* tree = btree_create(sizeof(int), sizeof(int));

        for (int i = 0; i < valno; ++i)
        {
                btree_insert(&tree, &i, &i, cmp_int);
        }

        for (int i = 0; i < valno; ++i)
        {
                btree_remove(&tree, &i, cmp_int);
        }

        btree_destroy(tree);

        return 0;
}

int
insert_delete_random()
{
        struct btree* tree = btree_create(sizeof(int), sizeof(int));

        for (int i = 0; i < valno; ++i)
        {
                btree_insert(&tree, &i, &i, cmp_int);
        }

        for (int i = 0; i < valno; ++i)
        {
                btree_remove(&tree, vals + i, cmp_int);
        }

        btree_destroy(tree);

        return 0;
}

int
insert_random_delete()
{
        struct btree* tree = btree_create(sizeof(int), sizeof(int));

        for (int i = 0; i < valno; ++i)
        {
                btree_insert(&tree, vals + i, vals + i, cmp_int);
        }

        for (int i = 0; i < valno; ++i)
        {
                btree_remove(&tree, &i, cmp_int);
        }

        btree_destroy(tree);

        return 0;
}
