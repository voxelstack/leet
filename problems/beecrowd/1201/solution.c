#pragma icanc include
#include <ds/bstree.h>
#include <ds/slice.h>
#include <leet.h>
#pragma icanc end

struct holder
{
        unsigned int data;
        struct bstree bst;
};

int
comparator(void* a, void* b)
{
        return container_of(a, struct holder, bst)->data
               - container_of(b, struct holder, bst)->data;
}

int
finder(void* val, void* n)
{
        return *(unsigned int*)val - container_of(n, struct holder, bst)->data;
}

void
infix(struct bstree* root, struct slice* out)
{
        if (!root)
                return;

        infix(root->_left, out);
        unsigned int value = container_of(root, struct holder, bst)->data;
        out->len += sprintf(out->data + out->len, "%u ", value);
        infix(root->_right, out);
}

void
prefix(struct bstree* root, struct slice* out)
{
        if (!root)
                return;

        unsigned int value = container_of(root, struct holder, bst)->data;
        out->len += sprintf(out->data + out->len, "%u ", value);
        prefix(root->_left, out);
        prefix(root->_right, out);
}

void
postfix(struct bstree* root, struct slice* out)
{
        if (!root)
                return;

        postfix(root->_left, out);
        postfix(root->_right, out);
        unsigned int value = container_of(root, struct holder, bst)->data;
        out->len += sprintf(out->data + out->len, "%u ", value);
}

void
bstree_left_remove(struct bstree** root, struct bstree* n)
{
        if (!n->_left)
                // No left subtree, lift the subtree on the right.
                _transplant(root, n, n->_right);
        else if (!n->_right)
                // No right subtree, lift the subtree on the left.
                _transplant(root, n, n->_left);
        else
        {
                // Two subtrees, find the predecessor.
                // Invariant: the predecessor doesn't have a right subtree (if
                // it did, the predecessor would have to be a node in that
                // subtree).
                struct bstree* y = bstree_last(n->_left);
                if (y != n->_left)
                {
                        // The predecessor is not immediately to the left,
                        // remove it.
                        _transplant(root, y, y->_left);
                        // Place the removed predecessor immediately to the
                        // left of n.
                        y->_left = n->_left;
                        y->_left->_parent = y;
                }

                // The predecessor is immediately to the left of n, lift it.
                _transplant(root, n, y);
                y->_right = n->_right;
                y->_right->_parent = y;
        }
}

void
tree_del(struct bstree* root)
{
        if (!root)
                return;

        tree_del(root->_left);
        tree_del(root->_right);
        free(container_of(root, struct holder, bst));
}

int
main()
{
        char in[32];
        char op[8];
        unsigned int val;

        struct slice* out = slice_make(sizeof(char), 512);
        ;
        struct bstree* root = NULL;

        while (fgets(in, sizeof(in), stdin))
        {
                // If the line we just read only has one word, it must be a
                // traversal instruction.
                if (sscanf(in, "%7s %u\n", op, &val) == 1)
                {
                        /*
                         * The second letter is different between all tree
                         * traversal instructions, we don't need to compare the
                         * whole word: I[N]FIXA, P[R]EFIXA, P[O]SFIXA
                         */
                        if (op[1] == 'N')
                                infix(root, out);
                        else if (op[1] == 'O')
                                postfix(root, out);
                        else
                                prefix(root, out);

                        // Trim final whitespace.
                        out->data[out->len - 1] = '\0';

                        printf("%s\n", out->data);
                        slice_clear(out);
                }
                else
                {
                        if (op[0] == 'I')
                        {
                                struct holder* node
                                    = calloc(1, sizeof(struct holder));
                                node->data = val;
                                bstree_insert(&root, &node->bst, comparator);
                        }
                        else if (op[0] == 'R')
                        {
                                struct bstree* node
                                    = bstree_search(root, &val, finder);
                                if (node)
                                        bstree_left_remove(&root, node);
                        }
                        else
                        {
                                if (bstree_search(root, &val, finder))
                                        printf("%u existe\n", val);
                                else
                                        printf("%u nao existe\n", val);
                        }
                }
        };
        slice_del(out);
        tree_del(root);

        return 0;
}
