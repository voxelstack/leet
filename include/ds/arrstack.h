#pragma once
#pragma icanc include
#include <ds/slice.h>
#include <leet.h>
#pragma icanc end

/**
 * @file arrstack.h
 *
 * `#include <ds/arrstack.h>`
 *
 * Array based stacks are backed by a slice. Data is stored in a contiguous
 * array for better cache locality. Some of the functions for manipulating the
 * underlying memory are aliases to slice functions. To keep the code clearer
 * you **should** use the aliases when working with array based data
 * structures, and you **should not** interact with the slice directly.
 * @see slice
 */

/**
 * @brief Initializes a stack and allocates its memory.
 *
 * Every call to slice_make **must** have a matching call to @ref slice_del to
 * release the managed memory.
 * @see slice_make
 *
 * @param p Handle to the slice.
 * @param el_size Size of each element.
 * @param el_no Number of elements for the initial allocation.
 */
struct slice* arrstack_make(size_t el_size, size_t el_no)
    __attribute__((alias("slice_make")));

/**
 * @brief Deallocates the memory managed by a stack created by
 * @ref arrstack_make
 * @see slice_del
 *
 * @param p Handle to the slice.
 */
void arrstack_del(struct slice* p) __attribute__((alias("slice_del")));

/**
 * @brief Returns whether the given stack is empty.
 * @see slice_empty
 *
 * @param p Handle to the slice.
 * @return Non-zero if the stack is empty.
 */
bool arrstack_empty(struct slice* p) __attribute__((alias("slice_empty")));

/**
 * @brief Pushes a value to the top of the stack *without checking its
 * capacity*.
 *
 * Copies the element stored at `el` to the top of the stack. *Does not* check
 * if there is enough space on the slice. For a safe version that grows the
 * slice when it's out of space, use @ref arrstack_spush.
 * @see slice_append
 *
 * @param p Handle to the slice.
 * @param el Pointer to the element to push.
 */
void arrstack_push(struct slice* p, void* el)
    __attribute__((alias("slice_append")));

/**
 * @brief Pushes a value to the top of the stack *checking its capacity*.
 *
 * Copies the element stored at `el` to the top of the stack. If the slice is
 * out of space, grows the allocated memory before pushing. When certain that
 * the slice has enough space, use @ref arrstack_push instead.
 * @see slice_sappend
 *
 * @param p Handle to the slice.
 * @param el Pointer to the element to push.
 */
void arrstack_spush(struct slice* p, void* el)
    __attribute__((alias("slice_sappend")));

/**
 * @brief Returns a pointer to the element at the top of the stack.
 *
 * Returns a *view* into the stack without copying the data.
 *
 * @param p Handle to the slice.
 */
data* arrstack_peek(struct slice* p) __attribute__((alias("slice_last")));

/**
 * @brief Pops and returns the value at top of the stack.
 *
 * *Copies* the value at the top of the stack to `dst`, then pops it.
 * **Should** be used when you need to keep the value outside the stack after
 * popping it. If you only need to read the value without copying it out of the
 * stack, use
 * @ref arrstack_peek followed by @ref arrstack_rwd to avoid a copy.
 *
 * @param p Handle to the slice.
 * @param dst Handle to *allocated* memory to hold the popped element.
 */
void
arrstack_pop(struct slice* p, data* dst)
{
        struct _slice* h = (struct _slice*)p;

        memcpy(dst, slice_last(p), h->el_size);
        slice_rwd(p, 1);
}

/**
 * @brief Pops and discards the value at top of the stack.
 *
 * Meant to be used if you only need to read the value at the top of the stack
 * *(without copying it out)*. Rewinds the stack to pop the value at the top
 * while avoiding a copy. **Should** probably be used after a @ref
 * arrstack_peek. For copying the popped value out of the stack use @ref
 * arrstack_pop instead.
 *
 * @param p Handle to the slice.
 */
void
arrstack_rwd(struct slice* p)
{
        slice_rwd(p, 1);
}

/**
 * @brief Pops and discards all values on the stack.
 * @see slice_clear
 *
 * @param p Handle to the slice.
 */
void arrstack_clear(struct slice* p) __attribute__((alias("slice_clear")));
