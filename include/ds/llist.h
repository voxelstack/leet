#pragma once
#pragma icanc include
#include <leet.h>
#pragma icanc end

/**
 * @file llist.h
 *
 * `#include <ds/llist.h>`
 */

/**
 * @brief A node in a [slingly linked
 * list](https://en.wikipedia.org/wiki/Linked_list).
 *
 * @dot
 * digraph llist {
 *   layout = "circo"
 *
 *   node [shape=record]
 *   head[label="head|<n>"]
 *   next[label="next|<n>"]
 *   null[label="nil" shape="diamond"]
 *
 *   head:n -> next
 *   next:n -> null
 * }
 * @enddot
 *
 * Nodes have no associated data and should be embedded onto containers
 * instead.
 * @see container_of
 */
struct llist
{
        struct llist* next;
};

/**
 * @brief Appends a node to *the end* of the list.
 *
 * @param p Handle to the list.
 * @param el Handle to the node to append.
 */
void
llist_append(struct llist* p, struct llist* el)
{
        while (p->next != NULL)
        {
                p = p->next;
        }
        p->next = el;
}

/**
 * @brief Finds an element on the list and returns a handle to it value, if it
 * exists.
 *
 * Finds the node that compares equally to the given element, if it exists.
 * Returns null otherwise. The comparator receives a pointer to the given
 * element, and a pointer to the key being compared, respectively.
 *
 * @param p Handle to the list.
 * @param el Handle to the element to search for.
 * @param cmp Search comparator.
 */
struct llist*
llist_search(struct llist* p, data* el, int (*cmp)(data*, data*))
{
        while (p != NULL)
        {
                if (cmp(el, p) == 0)
                {
                        return p;
                }
                p = p->next;
        }
        return NULL;
}

/**
 * @brief Finds and deletes a node from the list.
 *
 * Finds the node that compares equally to the given element and deletes it, if
 * it exists. Does not change the list if the node doesn't exist. The
 * comparator receives a pointer to the given element, and a pointer to the key
 * being compared, respectively.
 *
 * @param p Handle to the tree.
 * @param key Handle to the key to delete.
 * @param cmp Deletion comparator.
 * @return Whether or not the value was on the list.
 */
bool
llist_delete(struct llist** p, data* el, int (*cmp)(data*, data*))
{
        struct llist* curr = *p;
        if (cmp(el, curr) == 0)
        {
                *p = curr->next;
                return true;
        }

        struct llist* prev = NULL;
        while (curr != NULL)
        {
                if (cmp(el, curr) == 0)
                {
                        prev->next = curr->next;
                        return true;
                }
                prev = curr;
                curr = curr->next;
        }
        return false;
}

/**
 * @brief Loop through each element of the list.
 *
 * Loops through each element of the list, assigning the container of the
 * current element to an iterator.
 *
 * @param ctype Type of the container.
 * @param member Member of the container that holds the list node.
 * @param iterator Where to store the current element.
 * @param p Handle to the list.
 */
#define llist_foreach(ctype, member, iterator, p)                             \
        for (iterator = container_of(p, ctype, member);                       \
             (iterator)->member.next != NULL;                                 \
             iterator = container_of((iterator)->member.next, ctype, member))
