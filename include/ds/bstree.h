#pragma once
#pragma icanc include
#include <container_of.h>
#include <leet.h>
#pragma icanc end

/**
 * @file bstree.h
 *
 * `#include <ds/bstree.h>`
 */

/**
 * @brief A node in a [binary search
 * tree](https://en.wikipedia.org/wiki/Binary_search_tree).
 *
 * Nodes have no associated data and should be embedded onto containers
 * instead.
 * @see container_of
 */
struct bstree
{
        struct bstree* _parent; ///< Parent node (null if this is the root).
        struct bstree* _left;   ///< Left subtree.
        struct bstree* _right;  ///< Right subtree.
};

/**
 * @brief Replaces the subtree rooted at `u` with the subtree rooted at `v`.
 *
 * Given two subrees `u` and `v`, transplant `v` to the location of `u` by
 * replacing the link between `u`s parent and `u` with a link between `u`s
 * parent and `v`. The original subtree will not be unlinked and `u` will
 * retain a link to its parent.
 *
 * If a subtree is transplanted to the root, the root pointer will be updated.
 *
 * This is an internal function that **should not** be used directly.
 *
 * @param root Handle to the root of the tree.
 * @param u The subtree to be replaced.
 * @param v The subtree to transplant.
 */
void _transplant(struct bstree** root, struct bstree* u, struct bstree* v);

/**
 * @brief Inserts a node at the appropriate position on the tree.
 *
 * Given a node and a comparator, finds the appropriate position to insert the
 * node while keeping the tree sorted. The comparator receives pointers to the
 * `struct bstree` nodes being compared.
 *
 * If the tree is empty, the root pointer will be updated.
 *
 * @param root Handle to the root of the tree.
 * @param n The node to insert.
 * @param cmp Insertion comparator.
 */
void
bstree_insert(struct bstree** root, struct bstree* n, int (*cmp)(void*, void*))
{
        struct bstree* x = *root;
        struct bstree* y = NULL;
        while (x)
        {
                // Invariant: x is not empty, the new node must be inserted to
                // the left or to the right.
                y = x;
                if (cmp(n, x) < 0)
                        x = x->_left;
                else
                        x = x->_right;
        }

        n->_parent = y;
        if (!y)
                // The tree was empty.
                *root = n;
        else if (cmp(n, y) < 0)
                y->_left = n;
        else
                y->_right = n;
}

/**
 * @brief Finds a node on the tree, if it exists.
 *
 * Given a comparator finds a node that matches the given value, if it exists.
 * Returns `NULL` otherwise. The comparator receives a pointer to the given
 * value and a pointer to the `struct bstree` node being compared,
 * respectively.
 *
 * This function is included for completeness and **may** be used, but *it may
 * be more convenient to write your own search function.*
 *
 * @param n Handle to the tree to be searched.
 * @param value Value to search for.
 * @param cmp Search comparator. Compares `value` to a node on the tree.
 */
struct bstree*
bstree_search(struct bstree* n, void* value, int (*cmp)(void*, void*))
{
        while (n && cmp(value, n) != 0)
        {
                if (cmp(value, n) < 0)
                        n = n->_left;
                else
                        n = n->_right;
        }
        return n;
}

/**
 * @brief Finds the node associated with the minimum value on the tree.
 *
 * @param n Handle to the tree to be searched.
 * @return Handle to the minimum node on the tree.
 */
struct bstree*
bstree_first(struct bstree* n)
{
        while (n->_left)
                n = n->_left;
        return n;
}

/**
 * @brief Finds the node associated with the maximum value on the tree.
 *
 * @param n Handle to the tree to be searched.
 * @return Handle to the maximum node on the tree.
 */
struct bstree*
bstree_last(struct bstree* n)
{
        while (n->_right)
                n = n->_right;
        return n;
}

/**
 * @brief Finds the successor of a node.
 *
 * Used to traverse the tree in order.
 *
 * `for (struct bstree* it = bstree_first(&root); it; it = bstree_next(it))`
 *
 * @param n Handle to the current node.
 * @return Handle to the successor of the node.
 */
struct bstree*
bstree_next(struct bstree* n)
{
        if (n->_right)
                return bstree_first(n->_right);

        struct bstree* y = n->_parent;
        while (y && n == y->_right)
        {
                // Invariant: n has a parent and n is bigger than its parent.
                n = y;
                y = y->_parent;
        }
        return y;
}

/**
 * @brief Finds the predecessor of a node.
 *
 * Used to traverse the tree in reverse order.
 *
 * `for (struct bstree* it = bstree_last(&root); it; it = bstree_prev(it))`
 *
 * @param n Handle to the current node.
 * @return Handle to the predecessor of the node.
 */
struct bstree*
bstree_prev(struct bstree* n)
{
        if (n->_left)
                return bstree_last(n->_left);

        struct bstree* y = n->_parent;
        while (y && n == y->_left)
        {
                // Invariant: n has a parent and n is smaller than its parent.
                n = y;
                y = y->_parent;
        }
        return y;
}

/**
 * @brief Removes a node from the tree.
 *
 * Removes a node while keeping the tree sorted. The tree structure **may**
 * change as a result but all nodes are guaranteed to remain at the same
 * addresses.
 *
 * If the root is removed, the root pointer will be updated.
 *
 * @param root Handle to the root of the tree.
 * @param n Node to remove.
 */
void
bstree_remove(struct bstree** root, struct bstree* n)
{
        if (!n->_left)
                // No left subtree, lift the subtree on the right.
                _transplant(root, n, n->_right);
        else if (!n->_right)
                // No right subtree, lift the subtree on the left.
                _transplant(root, n, n->_left);
        else
        {
                // Two subtrees, find the successor.
                // Invariant: the successor doesn't have a left subtree (if it
                // did, the successor would have to be a node in that subtree).
                struct bstree* y = bstree_first(n->_right);
                if (y != n->_right)
                {
                        // The successor is not immediately to the right,
                        // remove it.
                        _transplant(root, y, y->_right);
                        // Place the removed successor immediately to the right
                        // of n.
                        y->_right = n->_right;
                        y->_right->_parent = y;
                }

                // The successor is immediately to the right of n, lift it.
                _transplant(root, n, y);
                y->_left = n->_left;
                y->_left->_parent = y;
        }
}

void
_transplant(struct bstree** root, struct bstree* u, struct bstree* v)
{
        if (!u->_parent)
                *root = v;
        else if (u == u->_parent->_left)
                u->_parent->_left = v;
        else
                u->_parent->_right = v;

        if (v)
                v->_parent = u->_parent;
}
