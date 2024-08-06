Binary search tree
==================

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
