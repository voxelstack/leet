Sort
====

A sequence of elements :math:`\langle a_1, a_2, \ldots, a_n \rangle` is considered *sorted* if its elements are such that :math:`\langle a_1 \le a_2 \le \ldots \le a_n \rangle`.
A **sorting algorithm** is then defined as an algorithm that, given a sequence :math:`\langle a_1, a_2, \ldots, a_n \rangle`, produces a permutation :math:`\langle a'_1, a'_2, \ldots, a'_n \rangle` such that :math:`\langle a'_1 \le a'_2 \le \ldots \le a'_n \rangle`.

Comparators
-----------

To determine whether or not a sequence :math:`\langle a, b \rangle` is sorted, we need a way to determine whether or not :math:`a \le b`.
Sorting algorithms that use :code:`<=` are limited to sorting numerical values in ascending order.

While we could associate numerical values with other data, sort the numerical values, and retrieve the associated data, it's more natural to work with arbitrary data stored as an array of bytes.
To make the sorting algorithm implementations usable across different types of abstract data, the sorting functions take a *comparator* as a parameter.

A comparator is a function that, given two pointers to abstract data :math:`a` and :math:`b`, returns :code:`1` if :math:`a \le b` and :code:`0` otherwise.
The simplest comparator compares two numerical values with the :code:`<=` operator.
Note that the comparator **must** return :code:`0` if :math:`a > b`, so :code:`b - a` will not sort numerical values in ascending order, since :math:`\{n | n < 0\}` is non-zero.

.. code-block:: c
    :caption: A comparator for sorting numerical values in ascending order.
    :emphasize-lines: 4

    int
    comparator(void* a, void* b)
    {
            return *(int*)a <= *(int*)b;
    }

External comparators allow you to interpret the data on the array as you see fit.
Consider an example where your array stores a list of music albums, each with a rating:

.. code-block:: c
    :caption: A music album with a rating.

    struct album
    {
            char* name;
            byte rating;
    };

With the right comparator, you can sort an array of albums by rating without the need for numerical keys:

.. code-block:: c
    :caption: A comparator for sorting numerical values in descending order.
    :emphasize-lines: 4

    int
    comparator(void* a, void* b)
    {
            return ((struct album*)a)->rating >= ((struct album*)b)->rating;
    }

API
---

.. doxygenfile:: alg/sort.h
    :sections: briefdescription detaileddescription

Functions
_________
.. doxygenfunction:: sort_merge

Internals
_________

.. caution::

    Internals are documented for completeness and to make the codebase easier to understand.
    However, they **should not** be used directly.

.. doxygenfunction:: _merge
