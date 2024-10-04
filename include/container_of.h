#pragma once
#pragma icanc include
#include <leet.h>
#pragma icanc end

/**
 * @file container_of.h
 *
 * `#include <container_of.h>`
 */

/**
 * @brief Calculates the address of a container given the address of a member.
 *
 * Given the address of a member embedded in a container structure, calculates
 * the address of the container.
 *
 * @param ptr Pointer to the member.
 * @param type Type of the container.
 * @param member Name of the member.
 * @return Address of the container.
 */
#define container_of(ptr, type, member)                                       \
        ({ (type*)(((byte*)(ptr)) - offsetof(type, member)); })
