#include "./tests.h"

#include <error.h>

int
main()
{
        start();

        test(set);
        test(set_no_error);
        test(set_literal);
        test(set_literal_no_error);
        test(del);
        test(propagate);
        test(propagate_no_source);
        test(propagate_no_destination);
        test(bubble);

        end();
}

int
set()
{
        struct error* error = NULL;
        error_set(&error, LINVAL, "error %s", "message");
        should(!eq(error, NULL), "error was not initialized");
        should(eq(error->code, LINVAL), "error code was not initialized");
        should(eq(strcmp(error->message, "error message"), 0),
               "message was not formatted");

        return 0;
}

int
set_no_error()
{
        error_set(NULL, LINVAL, "error message");

        return 0;
}

int
set_literal()
{
        struct error* error = NULL;
        error_set_literal(&error, LINVAL, "error message");
        should(!eq(error, NULL), "error was not initialized");
        should(eq(error->code, LINVAL), "error code was not initialized");
        should(eq(strcmp(error->message, "error message"), 0),
               "message was not assigned");

        return 0;
}

int
set_literal_no_error()
{
        error_set_literal(NULL, LINVAL, "error message");

        return 0;
}

int
del()
{
        struct error* error;
        error_set(&error, LINVAL, "error message");

        error_del(&error);
        should(eq(error, NULL), "error was not set to null");

        return 0;
}

int
propagate()
{
        struct error* error = NULL;
        struct error* tmp_error = NULL;
        error_set(&tmp_error, LINVAL, "error message");

        bool result = error_propagate(&tmp_error, &error);
        should(result, "propagate returned false but an error happened");
        should(!eq(error, NULL), "dst was not set");
        should(eq(error->code, LINVAL), "error code was not set");
        should(eq(strcmp(error->message, "error message"), 0),
               "error message was not set");

        return 0;
}

int
propagate_no_source()
{
        struct error* error = NULL;

        bool result = error_propagate(NULL, &error);
        should(!result, "propagate returned true but no error happened");
        should(eq(error, NULL), "propagate updated dst but no error happened");

        return 0;
}

int
propagate_no_destination()
{
        struct error* error = NULL;
        error_set(&error, LINVAL, "error message");

        bool result = error_propagate(&error, NULL);
        should(result, "propagate returned false but an error happened");
        should(eq(error, NULL),
               "propagate had no dst but didn't free the error");

        return 0;
}

int
bubble()
{
        struct error* error = NULL;
        error_set(&error, LINVAL, "error message");

        error_bubble(&error, NULL, 0);
        fail("bubble did not return");

        return 0;
}
