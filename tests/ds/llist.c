#include "../tests.h"

#include <container_of.h>
#include <ds/llist.h>

int
main()
{
        start();

        test(append);
        test(search_head);
        test(search_inner);
        test(delete_head);
        test(delete_inner);
        test(for_each);

        end();
}

struct holder
{
        int data;
        struct llist llist;
};

int
comparator(data* a, data* b)
{
        return container_of(a, struct holder, llist)->data
               - container_of(b, struct holder, llist)->data;
}

int
finder(data* a, data* b)
{
        return *(int*)a - container_of(b, struct holder, llist)->data;
}

#define make_list(nodes, head, n)                                             \
        for (int i = 0; i < n; ++i)                                           \
        {                                                                     \
                nodes[i].data = i;                                            \
                nodes[i].llist.next = NULL;                                   \
        }                                                                     \
        head = &nodes[0].llist;                                               \
        for (int i = 1; i < n; ++i)                                           \
        {                                                                     \
                llist_append(head, &nodes[i].llist);                          \
        }

int
append()
{
        struct holder nodes[100];
        struct llist* head;

        make_list(nodes, head, 100);

        int i = 0;
        struct holder* h = NULL;
        llist_foreach(struct holder, llist, h, head)
        {
                should(eq(i++, h->data), "element was not appended");
        }

        return 0;
}

int
search_head()
{
        struct holder nodes[2];
        struct llist* head;

        make_list(nodes, head, 2);

        int val = 0;
        struct llist* found = llist_search(head, &val, finder);

        should(eq(found, head), "search did not return list head");

        return 0;
}

int
search_inner()
{
        struct holder nodes[5];
        struct llist* head;

        make_list(nodes, head, 5);

        int val = 3;
        struct llist* found = llist_search(head, &val, finder);

        should(eq(found, head->next->next->next),
               "search did not return inner node");

        return 0;
}

int
delete_head()
{
        struct holder nodes[5];
        struct llist* head;

        make_list(nodes, head, 5);

        struct llist* succ = head->next;

        int val = 0;
        bool found = llist_delete(&head, &val, finder);

        should(found, "delete did not find list head");
        should(eq(head, succ), "delete did not update list head");

        int i = 1;
        struct holder* h = NULL;
        llist_foreach(struct holder, llist, h, head)
        {
                should(eq(i++, h->data), "resulting list was not correct");
        }

        return 0;
}

int
delete_inner()
{
        struct holder nodes[5];
        struct llist* head;

        make_list(nodes, head, 5);

        int val = 2;
        int vals[] = { 0, 1, 3, 4 };
        bool found = llist_delete(&head, &val, finder);

        should(found, "delete did not find inner node");

        int i = 0;
        struct holder* h = NULL;
        llist_foreach(struct holder, llist, h, head)
        {
                should(eq(vals[i++], h->data),
                       "resulting list was not correct");
        }

        return 0;
}

int
for_each()
{
        struct holder nodes[5];
        struct llist* head;

        make_list(nodes, head, 5);

        int i = 0;
        struct holder* h = NULL;
        llist_foreach(struct holder, llist, h, head)
        {
                should(eq(i++, h->data), "foreach order was not correct");
        }

        return 0;
}
