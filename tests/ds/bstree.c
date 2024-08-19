#include "../tests.h"

#include <ds/bstree.h>

int
main()
{
        start();

        test(insert);
        test(insert_duplicate);
        test(search);
        test(first);
        test(last);
        test(next);
        test(prev);
        test(remove_one_subtree);
        test(remove_adj_successor);
        test(remove_two_subtrees);
        test(remove_root);

        end();
}

struct holder
{
        int data;
        struct bstree bst;
};

int
comparator(void* a, void* b)
{
        return container_of(a, struct holder, bst)->data
               - container_of(b, struct holder, bst)->data;
}

#define link(p, c, l)                                                         \
        p._##l = &c;                                                          \
        c._parent = &p;

#define make_tree()                                                           \
        struct bstree* root = NULL;                                           \
        /* chosen by fair dice roll. */                                       \
        /* guaranteed to be random. */                                        \
        int a[] = { 9, 1, 3, 6, 4, 2, 8, 5, 7, 10 };                          \
        size_t arrno = sizeof(a) / sizeof(int);                               \
        do                                                                    \
        {                                                                     \
                struct holder* n = calloc(1, sizeof(struct holder));          \
                n->data = a[--arrno];                                         \
                bstree_insert(&root, &n->bst, comparator);                    \
        } while (arrno);

void
tree_del(struct bstree* n)
{
        if (!n)
                return;
        tree_del(n->_left);
        tree_del(n->_right);
        free(container_of(n, struct holder, bst));
}

int
finder(void* value, void* n)
{
        return *(int*)value - container_of(n, struct holder, bst)->data;
}

int
insert()
{
        struct holder n = { .bst = { 0 }, .data = 1 };
        struct holder nr = { .bst = { 0 }, .data = 2 };
        struct bstree* root = NULL;

        bool result = bstree_insert(&root, &n.bst, comparator);
        should(result, "insertion did not return true");
        should(eq(root, &n.bst), "root was not updated");

        result = bstree_insert(&root, &nr.bst, comparator);
        should(result, "insertion did not return true");
        should(eq(root, &n.bst), "root was changed when inserting child");
        should(eq(root->_right, &nr.bst), "child was not inserted");
        should(eq(nr.bst._parent, root),
               "parent was not updated when inserting child");

        return 0;
}

int
insert_duplicate()
{
        struct holder n = { .bst = { 0 }, .data = 1 };
        struct holder d = { .bst = { 0 }, .data = 1 };
        struct bstree* root = NULL;

        bstree_insert(&root, &n.bst, comparator);
        should(eq(root, &n.bst), "root was not updated");

        bool result = bstree_insert(&root, &d.bst, comparator);
        should(!result, "duplicate insertion did not return false");
        should(!root->_left && !root->_right,
               "tree was changed by duplicate insertion");

        return 0;
}

int
search()
{
        make_tree();

        int value;
        int found;
        struct bstree* node;

        value = 5;
        node = bstree_search(root, &value, finder);
        found = container_of(node, struct holder, bst)->data;
        should(eq(found, value), "returned value was not correct");

        value = 50;
        node = bstree_search(root, &value, finder);
        should(eq(node, NULL), "returned value was not null");

        tree_del(root);

        return 0;
}

int
first()
{
        struct holder n = { .bst = { 0 }, .data = 2 };
        struct holder nr = { .bst = { 0 }, .data = 1 };
        struct bstree* root = NULL;

        int first;
        bstree_insert(&root, &n.bst, comparator);
        first = container_of(bstree_first(root), struct holder, bst)->data;
        should(eq(first, 2),
               "first did not return the smallest value on the tree");

        bstree_insert(&root, &nr.bst, comparator);
        first = container_of(bstree_first(root), struct holder, bst)->data;
        should(eq(first, 1),
               "first did not return the smallest value on the tree");

        return 0;
}

int
last()
{
        struct holder n = { .bst = { 0 }, .data = 1 };
        struct holder nr = { .bst = { 0 }, .data = 2 };
        struct bstree* root = NULL;

        int last;
        bstree_insert(&root, &n.bst, comparator);
        last = container_of(bstree_last(root), struct holder, bst)->data;
        should(eq(last, 1),
               "last did not return the biggest value on the tree");

        bstree_insert(&root, &nr.bst, comparator);
        last = container_of(bstree_last(root), struct holder, bst)->data;
        should(eq(last, 2),
               "last did not return the biggest value on the tree");

        return 0;
}

int
next()
{
        make_tree();

        int exp = 1;
        for (struct bstree* it = bstree_first(root); it; it = bstree_next(it))
        {
                int data = container_of(it, struct holder, bst)->data;
                should(eq(data, exp++), "returned node was not the successor");
        }

        should(eq(exp, 11),
               "iterator was not stopped after traversing the tree");

        tree_del(root);

        return 0;
}

int
prev()
{
        make_tree();

        int exp = 10;
        for (struct bstree* it = bstree_last(root); it; it = bstree_prev(it))
        {
                int data = container_of(it, struct holder, bst)->data;
                should(eq(data, exp--),
                       "returned node was not the predecessor");
        }

        should(eq(exp, 0),
               "iterator was not stopped after traversing the tree");

        tree_del(root);

        return 0;
}

int
remove_one_subtree()
{
        struct holder p = { .bst = { 0 }, .data = 1 }; //    p
        struct holder z = { .bst = { 0 }, .data = 2 }; //  ┎ z ┒
        struct holder r = { .bst = { 0 }, .data = 3 }; //  0   r
        struct bstree* root = &p.bst;
        link(p.bst, z.bst, right);
        link(z.bst, r.bst, right);

        bstree_remove(&root, &z.bst);

        should(eq(p.bst._right, &r.bst), "p was not updated");
        should(eq(r.bst._parent, &p.bst), "r was not updated");

        return 0;
}

int
remove_adj_successor()
{
        struct holder p = { .bst = { 0 }, .data = 1 }; //     p
        struct holder z = { .bst = { 0 }, .data = 2 }; //  ┎  z  ┒
        struct holder y = { .bst = { 0 }, .data = 3 }; //  l   ┎ y ┒
        struct holder x = { .bst = { 0 }, .data = 4 }; //      0   x
        struct bstree* root = &p.bst;
        link(p.bst, z.bst, right);
        link(z.bst, y.bst, right);
        link(y.bst, x.bst, right);

        bstree_remove(&root, &z.bst);

        should(eq(p.bst._right, &y.bst), "p was not updated");
        should(eq(y.bst._parent, &p.bst), "y was not updated");

        return 0;
}

int
remove_two_subtrees()
{
        struct holder p = { .bst = { 0 }, .data = 1 }; //      p
        struct holder z = { .bst = { 0 }, .data = 2 }; //  ┎   z   ┒
        struct holder l = { .bst = { 0 }, .data = 3 }; //  l    ┎  r
        struct holder r = { .bst = { 0 }, .data = 3 }; //       y ┒
        struct holder y = { .bst = { 0 }, .data = 4 }; //         x
        struct holder x = { .bst = { 0 }, .data = 5 };
        struct bstree* root = &p.bst;
        link(p.bst, z.bst, right);
        link(z.bst, l.bst, left);
        link(z.bst, r.bst, right);
        link(r.bst, y.bst, left);
        link(y.bst, x.bst, right);

        bstree_remove(&root, &z.bst);

        should(eq(p.bst._right, &y.bst), "p was not updated");
        should(eq(y.bst._parent, &p.bst), "y->p was not updated");
        should(eq(y.bst._right, &r.bst), "y->r was not updated");
        should(eq(r.bst._parent, &y.bst), "r->p was not updated");
        should(eq(r.bst._left, &x.bst), "r->l was not updated");
        should(eq(x.bst._parent, &r.bst), "x->p was not updated");

        return 0;
}

int
remove_root()
{
        struct holder p = { .bst = { 0 }, .data = 1 }; //  ┎ p ┒
        struct holder l = { .bst = { 0 }, .data = 2 }; //  l   r
        struct holder r = { .bst = { 0 }, .data = 3 };
        struct bstree* root = &p.bst;
        link(p.bst, l.bst, left);
        link(p.bst, r.bst, right);

        bstree_remove(&root, &p.bst);

        should(eq(root, &r.bst), "root was not updated");

        return 0;
}
