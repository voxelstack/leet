#pragma once
#pragma icanc include
#include <leet.h>
#include <ds/slice.h>
#pragma icanc end

#define _btree_block_size 128

/**
 * @file btree.h
 *
 * `include <ds/btree.h>`
 */

/**
 * @brief A node in an in-memory
 * [B-tree](https://en.wikipedia.org/wiki/B-tree).
 */
struct btree
{
        struct slice* keys;     ///< The keys stored on this node.
        struct slice* values;   ///< The corresponding values.
        struct slice* children; ///< Pointers to each subtree.
};

struct _btree
{
        struct slice* keys;     ///< See @ref btree.
        struct slice* values;   ///< See @ref btree.
        struct slice* children; ///< See @ref btree.

        /// @privatesection
        size_t t; ///< Degree of the tree.
                  ///< How many elements fit on the btree block.
};

static bool leaf(struct btree* p);
static size_t keyno(struct btree* p);
static size_t find_key(struct btree* p, data* key, int (*cmp)(data*, data*));
static struct btree** child_at(struct btree* p, size_t idx);

static struct btree* btree_create_t(size_t key_size, size_t val_size,
                                    size_t t);

static void split_root(struct btree** p);
static void insert_non_full(struct btree* p, void* key, void* value,
                            int (*cmp)(void*, void*));

static bool delete_from_leaf(struct btree* x, size_t i);
static bool delete_from_internal(struct btree** x, data* key, size_t i,
                                 int (*cmp)(void*, void*));
static bool delete_from_subtree(struct btree** parent, void* key, size_t i,
                                int (*cmp)(void*, void*));

/**
 * @brief Initializes a btree.
 *
 * Every call to btree_create **must** have a matching call to
 * @ref btree_destroy to release the managed memory.
 *
 * @param key_size Size of each key.
 * @param val_size Size of each value.
 * @return Handle to the btree.
 */
struct btree*
btree_create(size_t key_size, size_t val_size)
{
        // TODO Benchmark this.
        size_t max_data_size
            = max(key_size, max(val_size, sizeof(struct btree*)));
        size_t t = _btree_block_size / (max_data_size * 2);

        return btree_create_t(key_size, val_size, t);
}

static struct btree*
btree_create_t(size_t key_size, size_t val_size, size_t t)
{
        struct _btree* h = malloc(sizeof(struct _btree));

        h->t = t;

        h->keys = slice_make(key_size, 2 * h->t - 1);
        h->values = slice_make(val_size, 2 * h->t - 1);
        h->children = slice_make(sizeof(struct btree*), 2 * h->t);

        return (struct btree*)h;
}

/**
 * @brief Deallocates the memory managed by a B-tree created with
 * @ref btree_create.
 *
 *
 * @param p Handle to the B-tree.
 */
void
btree_destroy(struct btree* p)
{
        slice_del(p->keys);
        slice_del(p->values);
        slice_del(p->children);
        free(p);
}

/**
 * @brief Inserts an entry into the tree.
 *
 * Finds the appropriate position and inserts the key-value pair while
 * preserving the B-tree properties.
 * The comparator receives a pointer to the given key, and a pointer
 * to the key being compared, respectively.
 * **May** update the root pointer.
 *
 * @param p Handle to the root of the tree.
 * @param key Handle to the key to insert.
 * @param value Handle to the value to insert.
 * @param cmp Insertion comparator.
 */
void
btree_insert(struct btree** p, void* key, void* value,
             int (*cmp)(void*, void*))
{
        struct _btree* h = (struct _btree*)*p;

        if (keyno(*p) == 2 * h->t - 1)
        {
                split_root(p);
                insert_non_full(*p, key, value, cmp);
        }
        else
        {
                insert_non_full(*p, key, value, cmp);
        }
}

/**
 * @brief Finds a key on the tree and returns a handle to its value, if it
 * exists.
 *
 * Finds the value associated with the given key, if it exists. Returns null
 * otherwise. The comparator receives a pointer to the given key, and a pointer
 * to the key being compared, respectively.
 *
 * @param p Handle to the tree.
 * @param key Handle to the key to search for.
 * @param cmp Search comparator.
 */
data*
btree_search(struct btree* p, data* key, int (*cmp)(data*, data*))
{
        size_t i = find_key(p, key, cmp);

        // If the current key is the one we're searching for then we
        // are done, return the value.
        if (i < keyno(p) && cmp(key, slice_at(p->keys, i)) == 0)
        {
                return slice_at(p->values, i);
        }
        // We haven't found the key yet and this node has no children.
        // This key is not on the tree.
        else if (leaf(p))
        {
                return NULL;
        }
        // We haven't found the key yet but this node has children. The
        // key could only be in the i-th child or under it, so keep
        // searching.
        else
        {
                return btree_search(*child_at(p, i), key, cmp);
        }
}

/**
 * @brief Finds and deletes an entry from the tree.
 *
 * Deletes the given key and its associated value from the tree, if it exists.
 * Does not change the tree if the key doesn't exist. The comparator receives a
 * pointer to the given key, and a pointer to the key being compared,
 * respectively.
 *
 * @param p Handle to the root of the tree.
 * @param key Handle to the key to delete.
 * @param cmp Deletion comparator.
 * @return Whether or not the value was on the original tree.
 */
bool
btree_remove(struct btree** p, void* key, int (*cmp)(void*, void*))
{
        struct btree* x = *p;

        size_t i = find_key(*p, key, cmp);

        // The key is on this node.
        if (i < keyno(x) && cmp(key, slice_at(x->keys, i)) == 0)
        {
                if (leaf(x))
                {
                        return delete_from_leaf(x, i);
                }
                return delete_from_internal(p, key, i, cmp);
        }

        // The key is not on this node ...
        // ... but it could be in the subtree rooted at the ith child.
        if (!leaf(x))
        {
                return delete_from_subtree(p, key, i, cmp);
        }
        // ... and this is a leaf. The key is not on the tree.
        return false;
}

static inline bool
leaf(struct btree* p)
{
        return p->children->len == 0;
}

static inline size_t
keyno(struct btree* p)
{
        return p->keys->len;
}

static size_t
childno(struct btree* p)
{
        return p->children->len;
}

static struct data*
key_at(struct btree* p, size_t idx)
{
        // TODO Most of the time we can know ahead of time whether or not the
        // index is in bounds. Is this if statement enough overhead to care
        // about?
        if (idx < keyno(p))
        {
                return slice_at(p->keys, idx);
        }

        return NULL;
}

static struct data*
val_at(struct btree* p, size_t idx)
{
        if (idx < keyno(p))
        {
                return slice_at(p->values, idx);
        }

        return NULL;
}

static struct btree**
child_at(struct btree* p, size_t idx)
{
        if (idx < childno(p))
        {
                return slice_at(p->children, idx);
        }
        return NULL;
}

static size_t
find_key(struct btree* p, data* key, int (*cmp)(data*, data*))
{
        size_t i = 0;
        while (i < keyno(p) && cmp(key, key_at(p, i)) > 0)
        {
                ++i;
        }
        return i;
}

static void
split_child(struct btree* p, size_t i)
{
        // Invariant: the i-th child is full.
        struct _btree* h = (struct _btree*)p;
        struct btree* child = *(struct btree**)slice_at(p->children, i);

        size_t key_size = ((struct _slice*)p->keys)->el_size;
        size_t val_size = ((struct _slice*)p->values)->el_size;
        struct btree* new_child = btree_create_t(key_size, val_size, h->t);

        // Copy the second half of the child to the new node (break the child
        // in half).
        memcpy(new_child->keys->data, slice_at(child->keys, h->t),
               h->t * key_size);
        memcpy(new_child->values->data, slice_at(child->values, h->t),
               h->t * val_size);
        if (!leaf(child))
        {
                memcpy(new_child->children->data,
                       slice_at(child->children, h->t), (h->t + 1) * val_size);
        }

        // Pull the median key/value from the child up to the parent.
        slice_insert(p->keys, key_at(child, h->t - 1), i);
        slice_insert(p->values, val_at(child, h->t - 1), i);

        // The node had 2t - 1 elements. After moving the median
        // element up to the parent we were left with 2t - 2 = 2(t - 1)
        // elements. So each new node now has t - 1 elements.
        // Meaning the original node lost (2t - 1) - (t - 1) =  t
        // elements.
        slice_rwd(child->keys, h->t);
        slice_rwd(child->values, h->t);
        if (!leaf(child))
        {
                slice_rwd(child->children, h->t);
        }

        // Insert the new child right after the median element we
        // pulled up.
        slice_insert(p->children, &new_child, i + 1);
}

static void
split_root(struct btree** p)
{
        struct btree* root = *p;
        struct _btree* h = (struct _btree*)root;
        struct _slice* keys = (struct _slice*)root->keys;
        struct _slice* values = (struct _slice*)root->values;

        // Parent the root to an empty note.
        struct btree* new_root
            = btree_create_t(keys->el_size, values->el_size, h->t);
        slice_append(new_root->children, &root);
        *p = new_root;

        split_child(new_root, 0);
}

static void
insert_non_full(struct btree* p, void* key, void* value,
                int (*cmp)(void*, void*))
{
        struct _btree* h = (struct _btree*)p;
        size_t i = find_key(p, key, cmp);

        if (leaf(p))
        {
                slice_insert(p->keys, key, i);
                slice_insert(p->values, value, i);
        }
        else
        {
                struct btree* child = *child_at(p, i);

                // If the child is full, split it.
                if (keyno(child) == 2 * h->t - 1)
                {
                        split_child(p, i);
                        // By splitting we just pulled up a key. If it's
                        // smaller than the one we want to insert, then we have
                        // to insert after the new key.
                        if (cmp(key, key_at(p, i)) > 0)
                        {
                                child = *child_at(p, i + 1);
                        }
                }
                insert_non_full(child, key, value, cmp);
        }
}

static struct btree**
merge(struct btree** root, size_t idx)
{
        struct btree* parent = *root;
        struct btree* target = *child_at(parent, idx);
        struct btree* victim = *child_at(parent, idx + 1);

        // Merge the key ...
        slice_append(target->keys, key_at(parent, idx));
        slice_append(target->values, val_at(parent, idx));

        // ... and the child into the previous child.
        for (int i = 0; i < keyno(victim); ++i)
        {
                data* key = key_at(victim, i);
                slice_append(target->keys, key);

                data* val = val_at(victim, i);
                slice_append(target->values, val);
        }
        if (!leaf(victim))
        {
                for (int i = 0; i < childno(victim); ++i)
                {
                        struct btree** child = child_at(victim, i);
                        slice_append(target->children, child);
                }
        }

        // Delete the key we merged.
        slice_delete_at(parent->keys, idx);
        slice_delete_at(parent->values, idx);

        // Delete the child we merged.
        btree_destroy(victim);
        slice_delete_at(parent->children, idx + 1);

        if (keyno(parent) == 0)
        {
                // The previous root is now empty, promote the child.
                *root = target;
                btree_destroy(parent);
                return root;
        }
        return child_at(parent, idx);
}

// TODO Linux rebalances in a way that doesn't need to steal nodes.
// https://github.com/torvalds/linux/blob/8bb886cb8f3a2811430ddb7d9838e245c57e7f7c/lib/btree.c#L535

static void
steal_prev(struct btree* parent, size_t idx)
{
        size_t prev_idx = idx - 1;
        struct btree* curr = *child_at(parent, idx);
        struct btree* prev = *child_at(parent, prev_idx);

        // Copy the key from x down into the child.
        // TODO slice_prepend
        slice_insert(curr->keys, key_at(parent, prev_idx), 0);
        slice_insert(curr->values, val_at(parent, prev_idx), 0);

        // Copy a key from the previous child up to x, replacing the
        // one we just moved down.
        slice_replace(parent->keys, slice_last(prev->keys), prev_idx);
        slice_replace(parent->values, slice_last(prev->values), prev_idx);
        slice_rwd(prev->keys, 1);
        slice_rwd(prev->values, 1);

        // Turn the last child of the previous node into the first

        // child of the child.
        if (!leaf(prev))
        {
                slice_insert(curr->children, slice_last(prev->children), 0);
                slice_rwd(prev->children, 1);
        }
}

static void
steal_next(struct btree* parent, size_t idx)
{
        struct btree* curr = *child_at(parent, idx);
        struct btree* next = *child_at(parent, idx + 1);

        // Copy the key from x down into the child.
        // TODO push_front
        slice_append(curr->keys, key_at(parent, idx));
        slice_append(curr->values, val_at(parent, idx));

        // Move a key from the next child up to x, replacing the
        // one we just moved down.
        slice_replace(parent->keys, slice_first(next->keys), idx);
        slice_replace(parent->values, slice_first(next->values), idx);
        slice_delete_at(next->keys, 0);
        slice_delete_at(next->values, 0);

        // Turn the first child of the next node into the last
        // child of the child.
        if (!leaf(next))
        {
                slice_append(curr->children, slice_first(next->children));
                slice_delete_at(next->children, 0);
        }
}

static bool
delete_from_leaf(struct btree* x, size_t i)
{
        slice_delete_at(x->keys, i);
        slice_delete_at(x->values, i);

        return true;
}

static bool
delete_from_internal(struct btree** x, data* key, size_t i,
                     int (*cmp)(void*, void*))
{
        struct btree* p = *x;
        struct _btree* h = (struct _btree*)p;

        struct btree** prev_child = child_at(p, i);
        struct btree** next_child = child_at(p, i + 1);

        if (keyno(*prev_child) >= h->t)
        {
                // Case 2a
                // Find the predecessor.
                struct btree** pred = prev_child;
                while (!leaf(*pred))
                {
                        pred = slice_last((*pred)->children);
                }

                data* pred_key = slice_last((*pred)->keys);
                data* pred_value = slice_last((*pred)->values);

                // Replace the node we want to remove with its predecessor.
                slice_replace(p->keys, pred_key, i);
                slice_replace(p->values, pred_key, i);

                // Recurse to delete the predecessor.
                return btree_remove(pred, pred_key, cmp);
        }
        if (keyno(*next_child) >= h->t)
        {
                // Case 2b
                // Find the successor.
                struct btree** succ = next_child;
                while (!leaf(*succ))
                {
                        succ = slice_first((*succ)->children);
                }

                data* succ_key = slice_first((*succ)->keys);
                data* succ_value = slice_first((*succ)->values);

                // Replace the node we want to remove with its succecessor.
                slice_replace(p->keys, succ_key, i);
                slice_replace(p->values, succ_key, i);

                // Recurse to delete the succecessor.
                return btree_remove(succ, succ_key, cmp);
        }

        // We know the root won't update, so it's not a problem, but I don't
        // like the & here.
        return btree_remove(merge(x, i), key, cmp);
}

static bool
delete_from_subtree(struct btree** parent, void* key, size_t i,
                    int (*cmp)(void*, void*))
{
        struct _btree* h = (struct _btree*)*parent;
        struct btree** subtree = child_at(*parent, i);

        // There are enough keys on the subtree.
        if (keyno(*subtree) >= h->t)
        {
                return btree_remove(subtree, key, cmp);
        }

        struct btree** prev_child = child_at(*parent, i - 1);
        struct btree** next_child = child_at(*parent, i + 1);

        // If a sibling has enough keys, steal one.
        if (prev_child != NULL && keyno(*prev_child) >= h->t)
        {
                steal_prev(*parent, i);
        }
        else if (next_child != NULL && keyno(*next_child) >= h->t)
        {
                steal_next(*parent, i);
        }
        // If a sibling also has t - 1 keys, merge.
        else if (prev_child != NULL)
        {
                subtree = merge(parent, i - 1);
        }
        else if (next_child != NULL)
        {
                subtree = merge(parent, i);
        }

        // Now there are enough keys on the subtree.
        return btree_remove(subtree, key, cmp);
}

void

btree_leaf_dot(struct btree* p, size_t depth, size_t i)

{

        if (keyno(p) == 0)

        {

                return;
        }

        printf(" n%x[label=\"", p);

        printf("%d:%d", ((int*)p->keys->data)[0],

               ((int*)p->values->data)[0]);
        for (size_t i = 1; i < keyno(p); ++i)

        {

                printf("|%d:%d", ((int*)p->keys->data)[i],

                       ((int*)p->values->data)[i]);
        }

        printf("\"]\n");
}

void

btree_node_dot(struct btree* p, size_t depth, size_t idx)

{

        if (leaf(p))

        {

                return btree_leaf_dot(p, depth, idx);
        }

        printf(" n%x[label=\"<c%u%u%u>", p, depth, idx, 0);

        for (size_t i = 0; i < keyno(p); ++i)

        {

                printf("|%d:%d|<c%u%u%u>", ((int*)p->keys->data)[i],

                       ((int*)p->values->data)[i], depth, idx, i + 1);
        }

        printf("\"]\n");

        for (size_t i = 0; i < keyno(p) + 1; ++i)

        {

                printf(" n%x:c%u%u%u -> n%x\n", p, depth, idx, i,

                       ((struct btree**)p->children->data)[i]);

                btree_node_dot(((struct btree**)p->children->data)[i],

                               depth + 1, i);
        }
}

void

btree_dot(struct btree* p)

{

        printf("digraph btree {\n");

        printf(" node [shape=record];\n");

        btree_node_dot(p, 0, 0);

        printf("}\n");
}
