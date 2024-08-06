Comparators
===========
While we could write generic data structures and algorithms by associating numerical values with other data, manipulating numerical values, and retrieving the associated data, it's more convenient to work with arbitrary data directly.
To make the code reusable across different types of abstract data, we can take a *comparator* as a parameter.

A comparator is a function :math:`cmp(a, b)` where :math:`a` and :math:`b` are pointers to abstract data, such that:

.. math::

    cmp(a,b) = \left\{\begin{array}{ll}
        n | n < 0, & \text{for } a < b \\
        0, & \text{for } a = b \\
        n | n > 0, & \text{for } a > b
    \end{array}\right.

The simplest comparator compares two numerical values with the :code:`-` operator.

.. code-block:: c
    :caption: A comparator for ordering numerical values in ascending order.
    :emphasize-lines: 4

    int
    comparator(void* a, void* b)
    {
            return *(int*)a - *(int*)b;
    }

External comparators allow you to interpret the data as you see fit.
Consider an example where the data is an album with a rating:

.. code-block:: c
    :caption: With the right comparator, albums can be sorted by rating.
    :emphasize-lines: 10

    struct album
    {
            char* name;
            byte rating;
    };

    int
    comparator(void* a, void* b)
    {
            return ((struct album*)a)->rating - ((struct album*)b)->rating;
    }

.. todo::

    * Proper definition of ordering and equality.
    * Ensure that comparators get inlined.
