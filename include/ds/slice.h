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
        byte* _data;      ///< Pointer to the managed array.
        size_t _capacity; ///< Size of the managed array in bytes.
        size_t _el_size;  ///< Size of each element in bytes.
        ptrdiff_t _ptr; ///< Write pointer. (Byte offset to the first free spot
                        ///< on the array.)
};

/**
 * @brief Initializes a slice and allocates its memory.
 *
 * Every call to slice_make **must** have a matching call to @ref slice_del to
 * release the managed memory.
 *
 * @param p Handle to the slice.
 * @param el_size Size of each element.
 * @param el_no Number of elements for the initial allocation.
 */
void
slice_make(struct slice* p, size_t el_size, size_t el_no)
{
        p->_capacity = el_size * el_no;
        p->_el_size = el_size;
        p->_ptr = 0;
        p->_data = malloc(p->_capacity);
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
        free(p->_data);
}

/**
 * @brief Returns whether the given slice is empty.
 *
 * @param p Handle to the slice.
 * @return Non-zero if the slice is empty.
 */
int
slice_empty(struct slice* p)
{
        return !p->_ptr;
}

/**
 * @brief Grows the underlying array by @ref _SLICE_SCALE_FACTOR.
 *
 * @param p Handle to the slice.
 */
void
slice_resize(struct slice* p)
{
        p->_capacity *= _SLICE_SCALE_FACTOR;
        p->_data = realloc(p->_data, p->_capacity);
}

/**
 * @brief Appends a value to the end of the slice *without checking its
 * capacity*.
 *
 * Copies the element stored at `el` to the end of the slice and advances the
 * write pointer. *Does not* check if there is enough space on the slice. For a
 * safe version that grows the slice when it's out of space, use
 * @ref slice_sappend.
 *
 * @param p Handle to the slice.
 * @param el Pointer to the element to append.
 */
void
slice_append(struct slice* p, void* el)
{
        memcpy(p->_data + p->_ptr, el, p->_el_size);
        p->_ptr += p->_el_size;
}

/**
 * @brief Appends a value to the end of the slice *checking its capacity*.
 *
 * Copies the element stored at `el` to the end of the slice and advances the
 * write pointer. If the slice is out of space, grows the allocated memory
 * before appending. When certain that the slice has enough space, use
 * @ref slice_append instead.
 * @see slice_resize
 *
 * @param p Handle to the slice.
 * @param el Pointer to the element to append.
 */
void
slice_sappend(struct slice* p, void* el)
{
        if (p->_ptr >= p->_capacity)
                slice_resize(p);
        slice_append(p, el);
}

/**
 * @brief Inserts a value into the given position of the slice.
 *
 * Copies the element stored at `el` into the given position of the slice,
 * shifting all existing elements to the right and advances the write pointer.
 * *Does not* check if there is enough space on the slice. For a safe version
 * that grows the slice when it's out of space, use
 * @ref slice_sinsert.
 *
 * @param p Handle to the slice.
 * @param el Pointer to the element to insert.
 * @param idx Index to insert the element at.
 */
void
slice_insert(struct slice* p, void* el, size_t idx)
{
        size_t at = idx * p->_el_size;
        memcpy(p->_data + at + p->_el_size, p->_data + at, p->_ptr - at);
        memcpy(p->_data + at, el, p->_el_size);
        p->_ptr += p->_el_size;
}

/**
 * @brief Inserts a value into the given position of the slice *checking its
 * capacity*.
 *
 * Copies the element stored at `el` into the given position of the slice,
 * shifting all existing elements to the right and advances the write pointer.
 * If the slice is out of space, grows the allocated memory before appending.
 * When certain that the slice has enough space, use
 * @ref slice_insert instead.
 * @see slice_resize
 *
 * @param p Handle to the slice.
 * @param el Pointer to the element to insert.
 * @param idx Index to insert the element at.
 */
void
slice_sinsert(struct slice* p, void* el, size_t idx)
{
        if (p->_ptr >= p->_capacity)
                slice_resize(p);
        slice_insert(p, el, idx);
}

/**
 * @brief Rewinds the write pointer, discarding elements at the end of the
 * slice.
 *
 * Moves the write pointer back by `rwd` elements. Further appends will
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
        p->_ptr -= p->_el_size * rwd;
}

/**
 * @brief Rewinds the write pointer to the start of the array, clearing the
 * slice.
 *
 * Moves the write pointer back to zero. Further appends will
 * override the previous elements.
 *
 * @param p Handle to the slice.
 */
void
slice_clear(struct slice* p)
{
        p->_ptr = 0;
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
void*
slice_at(struct slice* p, size_t idx)
{
        return p->_data + p->_el_size * idx;
}
