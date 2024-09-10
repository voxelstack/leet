[![beecrowd status](https://github.com/voxelstack/leet/actions/workflows/beecrowd.yaml/badge.svg)](https://judge.beecrowd.com/en/profile/578583)
[![ctest status](https://github.com/voxelstack/leet/actions/workflows/ctest.yaml/badge.svg)](#)
[![Documentation Status](https://readthedocs.org/projects/leet/badge/?version=latest)](https://leet.readthedocs.io/en/latest/?badge=latest)

# leet
This repo contains a well documented and tested header library for data structures and algorithms, assorted standalone programs, and solutions to leetcode problems. The solutions use the header library through icanc[^5].

## Getting started
1. Check out the [documentation](https://leet.readthedocs.io/en/latest/lib/index.html);
2. Browse the library code at [`include/`](https://github.com/voxelstack/leet/tree/main/include);
3. Browse the programs at [`programs/`](https://github.com/voxelstack/leet/tree/main/programs);
3. See the tests at [`tests/`](https://github.com/voxelstack/leet/tree/main/tests);
4. See the benchmarks at [`benchmarks/`](https://github.com/voxelstack/leet/tree/main/benchmarks);
5. *If you have solved a problem* and would like to compare your solution to mine or look for alternate solutions, see [my commentary for the solutions](https://leet.readthedocs.io/en/latest/problems/index.html#) (which includes links to the source code), or browse the source code at [`problems/`](https://github.com/voxelstack/leet/tree/main/problems/).

## Features
### Header library
On the `include/` directory you will find a header library with my implementations of data structures and algorithms.

### Solutions
The `problems/` directory has my solutions to leetcode problems, organized by judge and problem, along with testcases. On the documentation website, you can also see [my commentary on each solution](https://leet.readthedocs.io/en/latest/problems/index.html).

All solutions are tested with icanc[^5] using the testcases from the repo. When you see passing tests, it means that all solutions were accepted for the given testcases. I use the same [definition of accepted](https://www.udebug.com/faq#accepted-section) as udebug[^6]. I also test my solutions with udebug inputs using the icanc udebug integration. Those tests are not committed to the repo.

### Programs
On the `programs/` directory you can find the sources for standalone programs. Whenever I want to study, showcase, or document something that is not a leetcode problem, it goes here.
These are varied programs that don't need their own repo, built only for learning or showcasing skills.

### Documentation
All library code is documented with Doxygen[^1], which is then turned into a [Read the Docs website](https://leet.readthedocs.io/en/latest/?badge=latest) using sphinx[^2] and breathe[^3]. The programs are documented as well.

The setup is based on this [excellent guide](https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/) and adapted it to my needs. If you want to browse the documentation sources, they can be found in the `docs/` directory.

### Tests
All library code is tested with ctest[^4]. Non-trivial programs may be tested as well. You can find the test code in the `tests/` directory.

### Benchmarks
The performance charts on the documentation come from benchmarks on the `benchmarks/` directory.

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
