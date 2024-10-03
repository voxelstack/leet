#pragma once
#pragma icanc include
#include <leet.h>
#pragma icanc end

#include <stdarg.h>

/**
 * @file error.h
 *
 * `#include <error.h>`
 */

/**
 * @brief High level description of an error.
 */
enum error_code
{
        LINVAL, ///< Invalid value.
};

/**
 * @brief Information about an error.
 */
struct error
{
        enum error_code code; ///< High level description.
        char* message;        ///< Readable message.
};

/**
 * @brief Reports an unrecoverable error and exits the program.
 *
 * Exiting the program due to an unrecoverable error **should** be done through
 * this macro. **Should** be called with a readable error message.
 *
 * @param format Error message or format string.
 * @param ... Parameters for the format string.
 */
#define panic(...)                                                            \
        fprintf(stderr, "panicked at %s:%d:\n", __FILE__, __LINE__);          \
        fprintf(stderr, __VA_ARGS__);                                         \
        fprintf(stderr, "\n");                                                \
        exit(-1);

/**
 * @brief Reports a recoverable error with a formatted message.
 *
 * Functions that can fail should accept a handle to an error pointer as their
 * last parameter.
 *
 * This function creates an error and writes it to the given handle,
 * *transfering ownership to the caller*. After handling the error, the caller
 * **must** call @ref error_del or @ref error_propagate
 *
 * If you don't need a formatted string, use @ref error_set_literal instead.
 *
 * @param p Handle to write the error to.
 * @param error_code High level description of the error.
 * @param format Format string for the readable message.
 * @param ... Parameters for the format string.
 */
void
error_set(struct error** p, enum error_code code, const char* format, ...)
{
        if (p == NULL)
                return;

        va_list args;
        va_start(args, format);

        *p = malloc(sizeof(p));
        (*p)->code = code;
        // TODO Safe strings.
        (*p)->message = malloc(256);
        vsprintf((*p)->message, format, args);
}

/**
 * @brief Reports a recoverable error with a message.
 *
 * This is equivalent to @ref error_set but uses a given message instead of
 * formatting a string.
 *
 * @see error_set
 *
 * @param p Handle to write the error to.
 * @param error_code High level description of the error.
 * @param message Readable message.
 */
void
error_set_literal(struct error** p, enum error_code code, const char* message)
{
        if (p == NULL)
                return;

        *p = malloc(sizeof(p));
        (*p)->code = code;
        // TODO Safe strings.
        (*p)->message = malloc(256);
        strcpy((*p)->message, message);
}

/**
 * @brief Deallocates the memory backing an error created by @ref error_set or
 * @ref error_set_literal.
 *
 * @param p Handle to the error to deallocate.
 */
void
error_del(struct error** p)
{
        free((*p)->message);
        free(*p);
        *p = NULL;
}

/**
 * @brief Propagates a recoverable error to the destination, if it happened.
 *
 * When nesting functions that can fail, you **must** create a temporary error
 * for the subfunction call (the error handle passed to the parent function
 * could be `NULL`).
 *
 * This function can propagate the error to the parent function, while also
 * returning whether or not an error happened.
 *
 * ```c
 * int
 * can_error(struct error** error)
 * {
 *      struct error* tmp_error = NULL;
 *
 *      also_can_error(&tmp_error);
 *      if (error_propagate(&tmp_error, error))
 *              // The sub function failed.
 *              // Possibly clean up, then return from this function.
 *              // The error propagates upward.
 *              return -1;
 *
 *      // ... snip ...
 * }
 * ```
 *
 * When all you need is to propagate the error and return immediately
 * @ref error_bubble **may** be used to simplify this pattern.
 *
 * @param src The error to propagate.
 * @param dst Handle to propagate the error to.
 * @return Whether or not an error occurred.
 */
bool
error_propagate(struct error** src, struct error** dst)
{
        if (src == NULL || *src == NULL)
                return false;

        if (dst != NULL)
                *dst = *src;
        else
                error_del(src);

        return true;
}

/**
 * @brief Propagates a recoverable error immediately, returning a value from
 * the current function.
 *
 * It is a common pattern to check if an error occurred, and return from the
 * current function if it did.
 * In some cases there's no cleanup needed on the parent function and all you
 * have to do is propagate the error and return. This macro turns that pattern
 * into a single invocation.
 *
 * From this:
 * ```c
 * int
 * can_error(struct error** error)
 * {
 *      struct error* tmp_error = NULL;
 *
 *      also_can_error(&tmp_error);
 *      if (error_propagate(&tmp_error, error))
 *              return -1;
 *
 *      // ... snip ...
 * }
 * ```
 *
 * To this:
 * ```c
 * int
 * can_error(struct error** error)
 * {
 *      struct error* tmp_error = NULL;
 *
 *      also_can_error(&tmp_error);
 *      error_bubble(&tmp_error, error, -1);
 *
 *      // ... snip ...
 * }
 * ```
 *
 * If you don't need to return a value, use @ref error_bubble_void instead.
 *
 * @param src Source error.
 * @param dst Destination to propagate the error to.
 * @param ret Return value of the function.
 */
#define error_bubble(src, dst, ret)                                           \
        if (error_propagate((src), (dst)))                                    \
                return (ret);

/**
 * @brief Propagates a recoverable error immediately, returning from the
 * current function.
 *
 * This is equivalent to @ref error_bubble but doesn't return a value.
 *
 * @see error_bubble
 *
 * @param src Source error.
 * @param dst Destination to propagate the error to.
 */
#define error_bubble_void(src, dst)                                           \
        if (error_propagate((src), (dst)))                                    \
                return;
