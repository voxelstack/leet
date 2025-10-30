#pragma once
#pragma icanc include
#include <leet.h>
#pragma icanc end

/**
 * @file slice.h
 *
 * `#include <ds/slice.h>`
 */

/**
 * @brief How much a slice should grow if it runs out of space.
 *
 * If a slice is out of space when appending values with @ref slice_sappend,
 * the underlying array will grow by this factor.
 * @see slice_resize
 */
#define _SLICE_SCALE_FACTOR 2

/**
 * @brief Managed array with autoscaling capabilities.
 *        Building block for array-based data structures that **should not** be
 *        used directly.
 *
 * Slices keep track of the size of their elements, their current capacity and
 * the first free spot on their underlying array. Array based data structures
 * use slices to conveniently manipulate their underlying memory. Slices
 * **may** be used directly but, it is likely that you are looking for a higher
 * level data structure.
 */
struct slice
{
        byte* data; ///< Pointer to the underlying array.
        size_t len; ///< Number of elements currently on the slice.
};

/**
 * @brief Internal representation of a slice.
 *
 * Includes the public fields from @ref slice so user passed pointers can be
 * converted to internal ones.
 */
struct _slice
{
        byte* data; ///< See @ref slice.
        size_t len; ///< See @ref slice.

        /// @privatesection
        size_t capacity; ///< Allocated size in bytes.
        size_t el_size;  ///< Size of each element in bytes.
};

/**
 * @brief Initializes a slice and allocates its memory.
 *
 * Every call to slice_make **must** have a matching call to @ref slice_del to
 * release the managed memory.
 *
 * @param el_size Size of each element.
 * @param el_no Number of elements for the initial allocation.
 * @return Handle to the slice.
 */
struct slice*
slice_make(size_t el_size, size_t el_no)
{
        struct _slice* h = malloc(sizeof(struct _slice));
        h->el_size = el_size;
        h->capacity = el_no * el_size;
        h->data = malloc(h->capacity);
        h->len = 0;

        return (struct slice*)h;
}

/**
 * @brief Deallocates the memory managed by a slice created by @ref slice_make
 *
 *
 * @param p Handle to the slice.
 */
void
slice_del(struct slice* p)
{
        free(p->data);
        free(p);
}

/**
 * @brief Returns whether the given slice is empty.
 *
 * @param p Handle to the slice.
 * @return Whether the given slice is empty.
 */
bool
slice_empty(struct slice* p)
{
        return p->len == 0;
}

/**
 * @brief Returns whether the given slice is full.
 *
 * @param p Handle to the slice.
 * @return Whether the given slice is full.
 */
bool
slice_full(struct slice* p)
{
        struct _slice* h = (struct _slice*)p;

        return h->len * h->el_size >= h->capacity;
}

/**
 * @brief Returns a pointer to the element at the given index.
 *
 * Returns a *view* into the slice without copying the data. Indexing is done
 * by element type instead of bytes, so `idx` *does not* need to be multiplied
 * by the size of the elements.
 *
 * @param p Handle to the slice.
 * @param idx Index of the *element* to retrieve.
 */
data*
slice_at(struct slice* p, size_t idx)
{
        struct _slice* h = (struct _slice*)p;
        return h->data + idx * h->el_size;
}

/**
 * @brief Returns the first element of the slice.
 *
 * **Does not** check if the slice is empty.
 *
 * @param p Handle to the slice.
 * @return Pointer to the first element of the slice.
 */
data*
slice_first(struct slice* p)
{
        return p->data;
}

/**
 * @brief Returns the last element of the slice.
 *
 * **Does not** check if the slice is empty.
 *
 * @param p Handle to the slice.
 * @return Pointer to the last element of the slice.
 */
data*
slice_last(struct slice* p)
{
        struct _slice* h = (struct _slice*)p;

        return h->data + (h->len - 1) * h->el_size;
}

/**
 * @brief Discards elements at the end of the slice.
 *
 * Decrements the length of the slice by `rwd` elements. Further appends will
 * override the elements that got rewinded. *Does not* check the size of the
 * slice, `rwd` must be smaller than or equal to the number of elements
 * currently on the slice.
 *
 * @param p Handle to the slice.
 * @param rwd Number of elements to rewind.
 */
void
slice_rwd(struct slice* p, size_t rwd)
{
        p->len -= rwd;
}

/**
 * @brief Grows the underlying array by @ref _SLICE_SCALE_FACTOR.
 *
 * @param p Handle to the slice.
 */
void
slice_resize(struct slice* p)
{
        struct _slice* h = (struct _slice*)p;

        h->capacity *= _SLICE_SCALE_FACTOR;
        h->data = realloc(h->data, h->capacity);
}

/**
 * @brief Appends a value to the end of the slice *without checking its
 * capacity*.
 *
 * Copies the element stored at `el` to the end of the slice. *Does not* check
 * if there is enough space on the slice. For a safe version that grows the
 * slice when it's out of space, use @ref slice_sappend.
 *
 * @param p Handle to the slice.
 * @param el Pointer to the element to append.
 */
void
slice_append(struct slice* p, data* el)
{
        struct _slice* h = (struct _slice*)p;

        byte* ptr = h->data + h->len * h->el_size;
        memcpy(ptr, el, h->el_size);
        ++h->len;
}

/**
 * @brief Appends a value to the end of the slice *checking its capacity*.
 *
 * Copies the element stored at `el` to the end of the slice. If the slice is
 * out of space, grows the allocated memory before appending. When certain that
 * the slice has enough space, use @ref slice_append instead.
 * @see slice_resize
 *
 * @param p Handle to the slice.
 * @param el Pointer to the element to append.
 */
void
slice_sappend(struct slice* p, data* el)
{
        struct _slice* h = (struct _slice*)p;

        if (h->len * h->el_size >= h->capacity)
        {
                slice_resize(p);
        }
        slice_append(p, el);
}

/**
 * @brief Inserts a value into the given position of the slice.
 *
 * Copies the element stored at `el` into the given position of the slice,
 * shifting existing elements to the right. *Does not* check if there is enough
 * space on the slice. For a safe version that grows the slice when it's out of
 * space, use @ref slice_sinsert.
 *
 * @param p Handle to the slice.
 * @param el Pointer to the element to insert.
 * @param idx Index to insert the element at.
 */
void
slice_insert(struct slice* p, data* el, size_t idx)
{
        struct _slice* h = (struct _slice*)p;

        byte* ptr = slice_at(p, idx);
        if (idx < h->len)
        {
                byte* end = slice_at(p, p->len);
                memcpy(ptr + h->el_size, ptr, end - ptr);
        }
        memcpy(ptr, el, h->el_size);
        ++h->len;
}

/**
 * @brief Inserts a value into the given position of the slice *checking its
 * capacity*.
 *
 * Copies the element stored at `el` into the given position of the slice,
 * shifting existing elements to the right. If the slice is out of space, grows
 * the allocated memory before appending. When certain that the slice has
 * enough space, use @ref slice_insert instead.
 * @see slice_resize
 *
 * @param p Handle to the slice.
 * @param el Pointer to the element to insert.
 * @param idx Index to insert the element at.
 */
void
slice_sinsert(struct slice* p, data* el, size_t idx)
{
        struct _slice* h = (struct _slice*)p;
        if (h->len * h->el_size >= h->capacity)
        {
                slice_resize(p);
        }
        slice_insert(p, el, idx);
}

/**
 * @brief Deletes the element at a given position.
 *
 * Shifts existing elements to the right, overwriting the element to be
 * deleted. When preserving the order of elements is not important, use
 * @ref slice_delete_at_fast instead.
 *
 * @param p Handle to the slice.
 * @param idx Index of the element to retrieve.
 */
void
slice_delete_at(struct slice* p, size_t idx)
{
        struct _slice* h = (struct _slice*)p;

        byte* ptr = slice_at(p, idx);
        byte* end = slice_at(p, p->len);
        mempcpy(ptr, ptr + h->el_size, end - ptr);
        --h->len;
}

/**
 * @brief Deletes the element at a given position *without preserving element
 * order*.
 *
 * Copies the last element of the slice to the given position, then decrements
 * the length. Faster but does not preserve element order. If preserving the
 * order is important, use @ref slice_delete_at instead.
 *
 * @param p Handle to the slice.
 * @param idx Index of the element to retrieve.
 */
void
slice_delete_at_fast(struct slice* p, size_t idx)
{
        struct _slice* h = (struct _slice*)p;
        byte* ptr = slice_at(p, idx);
        memcpy(ptr, slice_last(p), h->el_size);
        slice_rwd(p, 1);
}

/**
 * @brief Sets the length to zero, clearing the slice.
 *
 * Sets the length to zero makikng it so further appends will override the
 * previous elements.
 *
 * @param p Handle to the slice.
 */
void
slice_clear(struct slice* p)
{
        p->len = 0;
}

/**
 * @brief Loop through each element of the array.
 *
 * Loops through each element of the array, assigning the current element to an
 * iterator of the given type.
 *
 * @param type Type of the iterator.
 * @param iterator Where to store the current element.
 * @param p Handle to the slice.
 */
#define slice_foreach(type, iterator, p)                                                  \
        for (                                                                             \
                iterator = (type)((p)->data;                                              \
                iterator < (type)((p)->data + (p)->len * ((struct _slice*)(p))->el_size); \
                ++iterator                                                                \
        )
