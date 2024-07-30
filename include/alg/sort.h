#pragma once
#pragma icanc include
#include <ds/slice.h>
#pragma icanc end

/**
 * @file sort.h
 *
 * `#include sort.h`
 *
 * Different algorithms and implementations for sorting a @ref slice *in
 * place* with the given comparator.
 */

/**
 * @brief Merges two sorted arrays into one sorted array.
 *
 * Given a slice containing two *sorted* arrays `a[p:q]` and `a[q+1:r]`, merge
 * them, and write the *sorted* result to `a[p:r]`.
 *
 * Uses an auxiliary slice as working memory. Data on the auxiliary slice *will
 * not be preserved*.
 *
 * This is an internal function that **should not** be used directly. You
 * probably want @ref sort_merge instead.
 *
 * @param a Handle to the slice.
 * @param w Handle to the auxiliary slice. **Must** have at least as much
 * capacity as a. **Must** have the same element size as `a`.
 * @param cmp Function that compares two void pointers `a` and `b`, returning
 * `1` if `a <= b` and 0 otherwise.
 * @param p Starting index of the first array.
 * @param q Ending index of the first array (the second array starts at `q +
 * 1`).
 * @param r Ending index of the second array.
 */
void _merge(struct slice* a, struct slice* w, int (*cmp)(void*, void*),
            size_t p, size_t q, size_t r);

/**
 * @brief Sorts an array *in place* using the [merge
 * sort](https://en.wikipedia.org/wiki/Merge_sort) algorithm.
 *
 * Given a slice containing an array `a[p:r]`, write the sorted array to
 * `a[p:r]`.
 *
 * Uses an auxiliary slice as working memory. Data on the auxiliary slice *will
 * not be preserved*.
 *
 * @param a Handle to the slice.
 * @param w Handle to the auxiliary slice. **Must** have at least as much
 * capacity as `a`. **Must** have the same element size as `a`.
 * @param cmp Function that compares two void pointers `a` and `b`, returning
 * `1` if `a <= b` and 0 otherwise.
 * @param p Starting index of the array.
 * @param r Ending index of the array.
 */
void
sort_merge(struct slice* a, struct slice* w, int (*cmp)(void*, void*),
           size_t p, size_t r)
{
        assert(a->_capacity <= w->_capacity
               && "Work slice does not have enough space.");
        assert(
            a->_el_size == w->_el_size
            && "Work slice element size does not match array element size.");

        if (p >= r)
                return;

        size_t q = floorf((p + r) / 2.0f);
        sort_merge(a, w, cmp, p, q);
        sort_merge(a, w, cmp, q + 1, r);
        _merge(a, w, cmp, p, q, r);
}

void
_merge(struct slice* a, struct slice* w, int (*cmp)(void*, void*), size_t p,
       size_t q, size_t r)
{
        size_t nl = q - p + 1;
        size_t nr = r - q;

        // Copy the left array to the first half of the work slice.
        memcpy(slice_at(w, p), slice_at(a, p), nl * a->_el_size);
        // Copy the right array to the second half of the work slice.
        memcpy(slice_at(w, q + 1), slice_at(a, q + 1), nr * a->_el_size);

        size_t i = 0;
        size_t j = 0;
        size_t k = p;

        while (i < nl && j < nr)
        {
                // if (l[i] <= r[j])
                if (cmp(slice_at(w, i + p), slice_at(w, j + q + 1)))
                        // a[k] = l[i]
                        memcpy(slice_at(a, k++), slice_at(w, i++ + p),
                               a->_el_size);
                else
                        // a[k] = r[j]
                        memcpy(slice_at(a, k++), slice_at(w, j++ + q + 1),
                               a->_el_size);
        }

        if (i < nl)
                /*
                 * If there are elements left on the left array, copy them to
                 * the sorted array. Note that even though the next if
                 * statement uses k we don't need to increment it, since
                 * `i < nl && j < nr` is always false.
                 */
                memcpy(slice_at(a, k), slice_at(w, i + p),
                       a->_el_size * (nl - i));

        if (j < nr)
                // If there are elements left on the right array, copy them to
                // the sorted array.
                memcpy(slice_at(a, k), slice_at(w, j + q + 1),
                       a->_el_size * (nr - j));
}
