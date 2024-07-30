[![beecrowd status](https://github.com/voxelstack/leet/actions/workflows/beecrowd.yaml/badge.svg)](https://judge.beecrowd.com/en/profile/578583)
[![ctest status](https://github.com/voxelstack/leet/actions/workflows/ctest.yaml/badge.svg)](#)
[![Documentation Status](https://readthedocs.org/projects/leet/badge/?version=latest)](https://leet.readthedocs.io/en/latest/?badge=latest)

# leet
This repo contains a well documented and tested header library for data structures and algorithms, along with my solutions to leetcode problems. The solutions use the header library through icanc[^5].

## Getting started
1. Check out the [documentation for the header library](https://leet.readthedocs.io/en/latest/lib/index.html);
2. Browse the library code at [`include/`](https://github.com/voxelstack/leet/tree/main/include);
3. See the library being used at [`tests/`](https://github.com/voxelstack/leet/tree/main/tests);
4. *If you have solved a problem* and would like to compare your solution to mine or look for alternate solutions, see [my commentary for the solutions](https://leet.readthedocs.io/en/latest/problems/index.html#) (which includes links to the source code), or browse the source code at [`problems/`](https://github.com/voxelstack/leet/tree/main/problems/).

## Features
### Header library
On the `include/` directory you will find a header library with my implementations of data structures and algorithms.

### Documentation
All library code is documented with Doxygen[^1], which is then turned into a [Read the Docs website](https://leet.readthedocs.io/en/latest/?badge=latest) using sphinx[^2] and breathe[^3].

The setup is based on this [excellent guide](https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/) and adapted it to my needs.

### Tests
All library code is tested with ctest[^4]. The tests are a good way to see the library in use, and you can find them in the `tests/` directory.

### Solutions
The `problems/` directory has my solutions to leetcode problems, organized by judge and problem, along with testcases. On the documentation website, you can also see [my commentary on each solution](https://leet.readthedocs.io/en/latest/problems/index.html).

All solutions are tested with icanc[^5] using the testcases from the repo. When you see passing tests, it means that all solutions were accepted for the given testcases. I use the same [definition of accepted](https://www.udebug.com/faq#accepted-section) as udebug[^6]. I also test my solutions with udebug inputs using the icanc udebug integration. Those tests are not committed to the repo.

## License
The header library is released under the MIT license and meant to be used as a learning resource. You are free to use it on your own leetcode solutions but I'd be happier (and you'd learn more) if you made your own.

You **should not** read the solutions before attempting to write your own, and you **must not** copy the solutions, entirely or in part.

---
> powered by [icanc](https://github.com/voxelstack/icanc)

[^1]: https://doxygen.nl/
[^2]: https://www.sphinx-doc.org/en/master/
[^3]: https://www.breathe-doc.org/
[^4]: https://cmake.org/cmake/help/latest/manual/ctest.1.html
[^5]: https://github.com/voxelstack/icanc/
[^6]: https://www.udebug.com/
