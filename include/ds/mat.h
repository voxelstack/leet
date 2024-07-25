#pragma once
#pragma icanc include
#include <leet.h>
#pragma icanc end

/**
 * @file mat.h
 *
 * `#include <ds/mat.h>`
 */

/**
 * @brief View into memory representing a matrix.
 *
 * Utilities to manipulate a row-major matrix stored on a contiguous block of
 * memory.
 */
struct mat
{
        size_t _ino;     ///< Number of columns.
        size_t _jno;     ///< Number of rows.
        size_t _el_size; ///< Size of each element in bytes.
        byte* _data;     ///< Pointer to the underlying array.
};

/**
 * @brief Initializes a matrix and allocates its memory.

 * Every call to mat_make **must** have a matching call to @ref mat_del to
 * release the managed memory.
 *
 * @param p Handle to the slice.
 * @param el_size Size of each element.
 * @param ino Number of columns.
 * @param jno Number of rows.
 */
void
mat_make(struct mat* p, size_t el_size, size_t ino, size_t jno)
{
        p->_el_size = el_size;
        p->_ino = ino;
        p->_jno = jno;
        p->_data = malloc(ino * jno * el_size);
}

/**
 * @brief Deallocates the memory backing a matrix created by @ref mat_make
 *
 * @param p Handle to the matrix.
 */
void
mat_del(struct mat* p)
{
        free(p->_data);
}

/**
 * @brief Returns the byte offset to the ij-th element on the underlying array.
 *
 * @param p Handle to the matrix.
 * @param i
 * @param j
 */
size_t
mat_idxof(struct mat* p, size_t i, size_t j)
{
        return (p->_jno * i * p->_el_size) + (j * p->_el_size);
}

/**
 * @brief Sets the value of the ij-th element of the matrix.
 *
 * Copies the element stored at `el` to the position of the ij-th element of
 * the matrix.
 *
 * @param p Handle to the matrix.
 * @param i
 * @param j
 * @param el Pointer to the element to append.
 */
void
mat_set(struct mat* p, size_t i, size_t j, void* el)
{
        size_t idx = mat_idxof(p, i, j);
        memcpy(p->_data + idx, el, p->_el_size);
}

/**
 * @brief Returns a pointer to the element at the ij-th index.
 *
 * Returns a *view* into the underlying memory.
 *
 * @param p Handle to the matrix.
 * @param i
 * @param j
 * @param el Pointer to the element to append.
 */
void*
mat_at(struct mat* p, size_t i, size_t j)
{
        size_t idx = mat_idxof(p, i, j);
        return p->_data + idx;
}
