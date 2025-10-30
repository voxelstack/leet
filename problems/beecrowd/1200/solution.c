#pragma icanc include
#include <ds/bstree.h>
#include <ds/slice.h>
#include <leet.h>
#pragma icanc end

struct holder
{
        char data;
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
        return *(char*)val - container_of(n, struct holder, bst)->data;
}

void
infix(struct bstree* root, struct slice* out)
{
        if (!root)
                return;

        infix(root->_left, out);
        slice_append(out, &container_of(root, struct holder, bst)->data);
        slice_append(out, &" ");
        infix(root->_right, out);
}

void
prefix(struct bstree* root, struct slice* out)
{
        if (!root)
                return;

        slice_append(out, &container_of(root, struct holder, bst)->data);
        slice_append(out, &" ");
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
        slice_append(out, &container_of(root, struct holder, bst)->data);
        slice_append(out, &" ");
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
        char in[16];
        char op[8];
        char val;

        struct bstree* root = NULL;
        struct slice* out = slice_make(sizeof(char), 512);

        while (fgets(in, sizeof(in), stdin))
        {
                // If the line we just read only has one word, it must be a
                // traversal instruction.
                if (sscanf(in, "%7s %c\n", op, &val) == 1)
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
                        else
                        {
                                if (bstree_search(root, &val, finder))
                                        printf("%c existe\n", val);
                                else
                                        printf("%c nao existe\n", val);
                        }
                }
        };
        slice_del(out);
        tree_del(root);

        return 0;
}
