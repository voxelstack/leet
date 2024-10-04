container_of
============

Usage
-----
:code:`container_of` allows data structure code to be generic across multiple data types.
Data structures can be implemented without being tied to any data, and then embedded onto a container.

.. code-block:: c
    :caption: This binary search tree node holds no data. Instead it's embedded onto an album.
    :emphasize-lines: 12

    struct bstree
    {
            struct bstree* _parent;
            struct bstree* _left;
            struct bstree* _right;
    };

    struct album
    {
            char* name;
            byte rating;
            struct bstree bst;
    };

The data structure code can then manipulate structures embedded onto the container, and :code:`container_of` can be used to retrieve the data.

.. code-block:: c
    :caption: Retrieve the data associated with a tree node.
    :emphasize-lines: 2

    struct bstree* node = bstree_first(tree);
    struct album* album = container_of(node, struct album, bst);
    printf("%s\n", album->name);

How it works
------------
Suppose a :code:`struct album a` is stored in memory at address :code:`&a`.
The value of :code:`a.bst` will be stored at :code:`&a.bst`, a certain offset :code:`bst_off` from the start of the data.
If we have the address of :code:`a.bst`, we can figure out the address of :code:`a`: :code:`&a = &a.bst - bst_off`.

.. code-block::

        &a                &a.bst = &a + bst_off
        v                 v
    ----+--------+--------+------------------------+----
     ...| name   | rating | bst                    |...
    ----+--------+--------+------------------------+----
        <-bst_off --------|

That is exactly what :code:`container_of` does:

.. code-block:: c

    #define container_of(ptr, type, member)                       \
            ({ (type*)(((byte*)(ptr)) - offsetof(type, member)); })

:code:`((byte*)(&a.bst))`
    Since the offset of a struct member is given in bytes, we cast the pointer to a :code:`byte*` before doing arithmetic.

:code:`offsetof(struct album, bst)`
    How many bytes away from the start of a struct of type :code:`struct album` is the member :code:`bst` stored.
    This is the value of :code:`bst_off`.

:code:`(((byte*)(&a.bst)) - offsetof(struct album, bst))`
    :code:`&a.bst - bst_off` gives us :code:`&a`, the address of the container we wanted to find.

:code:`(struct album*)(((byte*)(&a.bst)) - offsetof(struct album, bst))`
    Cast the result back to the type of the container.

API
---

.. doxygenfile:: container_of.h
    :sections: briefdescription detaileddescription

Defines
_______

.. doxygendefine:: container_of
