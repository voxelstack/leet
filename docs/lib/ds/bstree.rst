Binary search tree
==================

Inserting data into a bstree
----------------------------
There are two main ways to insert data into the tree: using the provided insertion function along with a comparator, or writing your own insertion function.
Writing your own insertion function is more performant since it avoids using a function pointer.

.. chart:: _charts/bench.bstree_insert_fnptr.json

   1.000.000 random numbers inserted into a bstree (-O0, 100 runs)

API
---

.. doxygenfile:: ds/bstree.h
    :sections: briefdescription detaileddescription

Handle
______

.. doxygenstruct:: bstree
    :members:

Functions
_________

.. doxygenfunction:: bstree_link
.. doxygenfunction:: bstree_insert
.. doxygenfunction:: bstree_search
.. doxygenfunction:: bstree_first
.. doxygenfunction:: bstree_last
.. doxygenfunction:: bstree_next
.. doxygenfunction:: bstree_prev
.. doxygenfunction:: bstree_remove

Internals
_________

.. caution::

    Internals are documented for completeness and to make the codebase easier to understand.
    However, they **should not** be used directly.

.. doxygenfunction:: _transplant
