calc
====

A stack machine capable of adding, subtracting, multiplying, and dividing.

Operations
----------
A :code:`|` indicates the program is waiting for an operation. Operations are defined by a single character, possibly with one integer parameter separated by a space.

help
    .. code-block::
        :caption: Print a help message.

        | h

exit
    .. code-block::
        :caption: Exit the program.

        | .

push
    .. code-block::
        :caption: Push a value onto the stack.

        | < 123

pop
    .. code-block::
        :caption: Print the value at the top of the stack and pop it.

        | >

op
    Pop the two values on top of the stack, execute an operation, and push the result back onto the stack.

    .. code-block::
        :caption: 3 * 1 + 4

        | < 1
        | < 4
        | +
        | < 3
        | *
        | >

Outputs
-------
Values printed without a :code:`|` are outputs.

number
    The result of a pop.
nil
    There is nothing on the stack.
err
    Error executing the last command. The stack was not changed.
