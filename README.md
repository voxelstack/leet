[![beecrowd status](https://github.com/voxelstack/leet/actions/workflows/beecrowd.yaml/badge.svg)](https://judge.beecrowd.com/en/profile/578583)
[![ctest status](https://github.com/voxelstack/leet/actions/workflows/ctest.yaml/badge.svg)](#)
[![Documentation Status](https://readthedocs.org/projects/leet/badge/?version=latest)](https://leet.readthedocs.io/en/latest/?badge=latest)

# leet
leet includes a well documented and tested header library for data structures and algorithms, along with my solutions to leetcode problems.

## Features
### Header library
On the `include/` directory you will find a header library with my implementations of data structures and algorithms.

### Documentation
All library code is documented with [Doxygen](https://doxygen.nl/), which is then turned into a [Read the Docs website](https://leet.readthedocs.io/en/latest/?badge=latest) using [sphinx](https://www.sphinx-doc.org/en/master/) and [breathe](https://www.breathe-doc.org/).

I set it up by following this [excellent guide](https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/#catcutifier-docs-cmakelists-txt) and adapting it to my needs.

### Tests
All library code is tested with [ctest](https://cmake.org/cmake/help/latest/manual/ctest.1.html). You can find the tests on the `tests/` directory, they are a good way to see the library in use.

### Solutions
The `problems/` directory has my solutions to leetcode problems, organized by judge and problem, along with testcases. On the documentation website, you can also see [my commentary on each solution](https://leet.readthedocs.io/en/latest/problems/index.html).

All solutions are tested with [icanc](https://github.com/voxelstack/icanc) using the testcases from the repo. When you see passing tests, it means that all solutions were accepted for the given testcases. I use the same [definition of accepted](https://www.udebug.com/faq#accepted-section) as [udebug](https://www.udebug.com/).

## License
The header library is released under the MIT license and meant to be used as a learning resource. You are free to use it on your own leetcode solutions but I'd be happier (and you'd learn more) if you made your own.

You **should not** read the solutions before attempting to write your own, and you **must not** copy the solutions, entirely or in part.

> powered by [icanc](https://github.com/voxelstack/icanc)
