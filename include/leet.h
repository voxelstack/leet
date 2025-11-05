#pragma once

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;
typedef void data;

/**
 * **May cause multiple evaluations.**.
 *
 * Not a rabbit hole I want to go down right now.
 * https://github.com/torvalds/linux/blob/master/include/linux/minmax.h
 */
#define max(a, b) ((a) > (b) ? (a) : (b))
