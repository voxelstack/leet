Error handling
==============

leet offers utilities to deal with two groups of errors: *recoverable* and *unrecoverable*.

An error is recoverable when it can be treated by your code and execution is allowed to continue.
For example if you're making a `REPL <https://en.wikipedia.org/wiki/Read%E2%80%93eval%E2%80%93print_loop>`_, getting a wrong instruction is not the end of the world: your code realizes it can't execute it, lets the user know, and waits for the next instruction.

An error is unrecoverable when it is impossible to continue execution after it happens.
For example if your code tries to access the 100th element of a slice that only has capacity for 10 elements, execution cannot continue.

.. note::

    - The separation between recoverable and unrecoverable errors is inspired by `Rust's error handling <https://doc.rust-lang.org/book/ch09-00-error-handling.html>`_.
    - The error handling system is inspired by `GLib's error handling <https://docs.gtk.org/glib/error-reporting.html>`_.

Unrecoverable errors
--------------------

When you realize something went horribly wrong and your program cannot continue, you should exit with a panic.
A panic **must** be triggered with a readable message describing the error.

Using our slice example from before:

.. code-block:: c
    :caption: Safe slice access that triggers a panic if the index is out of bounds.
    :emphasize-lines: 7

    void*
    slice_sat(struct slice* p, size_t idx)
    {
            size_t offset = p->_el_size * idx;

            if (offset >= p->_capacity)
                panic("access out of bounds: slice capacity is %u but offset was %u.", p->_capacity, offset);

            return p->_data + offset;
    }

The panic will print your message to :code:`stderr` along with where it happened, and exit with a non-zero code.

.. code-block:: text
    :caption: Panic location and readable error message.

    panicked at include/ds/slice.h:190:
    slice access out of bounds: slice capacity is 10 but offset was 100.

Recoverable errors
------------------

Reporting errors
________________

When a function fails with an error that does not require stopping execution, you **may** report the error to the caller so it can be handled.
It is equivalent to a throw in languages that have exceptions.
Functions that can report a recoverable error **should** accept a handle to an error pointer as the last parameter.

Using our REPL example:

.. code-block:: c
    :caption: Binary operation that reports an error if there is only one operand.
    :emphasize-lines: 6

    void
    bin_op(struct slice* s, char op, struct error** error)
    {
            if (s->_ptr < s->_el_size * 2)
            {
                    error_set(error, LINVAL, "%c is a binary operation but the stack only has one value.", op);
            }
            else
            {
                    // We have at least two values, execute the operation.
                    // ... snip ...
            }
    }

.. attention::

    Reporting an error allocates memory and transfers its ownership to the caller.
    That means the caller **must not** allocate memory for the report, but **must** free the memory after handling it.

Handling errors
_______________

To get error reports you **must** pass a valid pointer to a function's :code:`error` parameter.
Calling a function with a valid error pointer and checking if the pointer was set is equivalent to a try-catch in languages that support exceptions.
The error pointer will be set if the function encounters an error, or *left unchanged* if the function executes successfully.

.. code-block:: c
    :caption: If the pointer is set, the function encountered an error.
    :emphasize-lines: 5, 9

    // ... snip ...
    struct error* error = NULL;

    bin_op(stack, op, &error);
    if (error != NULL)
    {
            // Let the user know the operation could not be executed.
            // ... snip ...
            error_del(&error);
    }

If an error can be safely ignored, you **may** call the function with a null pointer.
It is equivalent to a try-catch with an empty catch block in languages that have exceptions.
This does not mean the error will not be handled, it means the error can be handled by doing nothing.

.. code-block:: c
    :caption: A null pointer signifies that the error is handled by doing nothing.
    :emphasize-lines: 15

    bool
    update(int id, struct error** error)
    {
            if (!exists(id))
            {
                    set_error(error, LNOENT, "no entity with the given id: %d", id);
                    return false;
            }
            // ... snip ...
    }

    void
    update_if_exists(int id)
    {
            update(id, NULL);
    }

When nesting functions that can report errors it is important to not reuse the error pointer provided to the parent function, as it may be null.
Instead you **should** create a temporary error pointer.

.. code-block:: c
    :caption: Call the child function with a temporary error pointer.
    :emphasize-lines: 5, 10

    void
    parent(struct error** error)
    {
            struct error* tmp_error = NULL;
            child(&tmp_error);
            if (tmp_error != NULL)
            {
                    // Handle error from the child function.
                    // ... snip ...
                    // Call error_del or error_propagate.
            }
    }

Propagating errors
__________________

When a caller cannot handle an error from a nested function call, you **may** propagate the error upwards with :code:`error_propagate`.
It is equivalent to calling a function that throws exceptions outside of a try-catch block or catching and re-throwing in languages that have exceptions.
When propagating an error you don't need to worry about ownership, :code:`error_propagate` is smart enough to free the error in cases where the caller chose to ignore it or transfer ownership otherwise.

.. code-block:: c
    :caption: This error cannot be handled here but may be handled by the caller of :code:`parse`, propagate it.
    :emphasize-lines: 12

    bool
    parse(const char* src, struct slice* tokens, struct error** error)
    {
            struct error* tmp_error = NULL;

            // ... snip ...
            parse_ident(src, ptr, tokens, &tmp_error);
            if (tmp_error != NULL)
            {
                    // We need an identifier but couldn't parse one,
                    // there's nothing we can do to fix that.
                    error_propagate(&tmp_error, error);
                    return false;
            }
    }


If all you need is to propagate an error and return, :code:`error_bubble` can help avoid repeating the if statement from the previous example:

.. code-block:: c
    :caption: Propagate and return false immediately on error.
    :emphasize-lines: 8

    bool
    parse(const char* src, struct slice* tokens, struct error** error)
    {
            struct error* tmp_error = NULL;

            // ... snip ...
            parse_ident(src, ptr, tokens, &tmp_error);
            error_bubble(&tmp_error, error, false);
    }


Performance
___________

Reporting errors requires a memory allocation and string formatting.
For more performant reports you **may** avoid the string formatting with :code:`error_set_literal`, or fallback to integer error codes.

TODO
----

.. todo::

    - Safe strings.

.. todo::

    - Warnings and assertions when error functions are used incorrectly.

API
---
.. doxygenfile:: error.h
    :sections: briefdescription detaileddescription

Enums
_____
.. doxygenenum:: error_code

Handle
______
.. doxygenstruct:: error
    :members:

Functions
_________
.. doxygenfunction:: error_set
.. doxygenfunction:: error_set_literal
.. doxygenfunction:: error_del
.. doxygenfunction:: error_propagate

Macros
______
.. doxygendefine:: panic
.. doxygendefine:: error_bubble
.. doxygendefine:: error_bubble_void
