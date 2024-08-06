Sort
====

A sequence of elements :math:`\langle a_1, a_2, \ldots, a_n \rangle` is considered *sorted* if its elements are such that :math:`\langle a_1 \le a_2 \le \ldots \le a_n \rangle`.
A **sorting algorithm** is then defined as an algorithm that, given a sequence :math:`\langle a_1, a_2, \ldots, a_n \rangle`, produces a permutation :math:`\langle a'_1, a'_2, \ldots, a'_n \rangle` such that :math:`\langle a'_1 \le a'_2 \le \ldots \le a'_n \rangle`.

.. seealso::

    Sorting algorithms can sort arbitrary data using :doc:`../overview/comparators`.

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
