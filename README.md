# Bronto

This repository holds infrastructure to help automate refactoring and migrations
of C++ code. The library contains annotations that can be added to your existing
C++ code to indicate transformations which you desire to happen in the
background. The Bronto refactoring tool (not included here) is responsible for
understanding these annotations and producing the corresponding changes. If this
interests you, please contact us at
[contact@brontosource.dev](mailto:contact@brontosource.dev).

The infrastructure included in this repository is a single header file. It is
designed to have no run-time cost and an immeasurably small compile-time
footprint.

# Build Requirements

This project requires compiling with C++17 or a later version.

Bronto can be built with either Bazel or CMake.

| Build system | Minimum supported version |
| ------------ | ------------------------- |
| Bazel        | 8.1.1                     |
| CMake        | 3.10.0                    |

## Bazel

Depend on the project via the
[Bazel Central Registry](https://registry.bazel.build/), and add
`@bronto//:bronto` to the appropriate `deps`. You can then include
`"bronto/bronto.hpp"`.

## CMake

This project is `FetchContent`-friendly. Our releases will be tagged with with
the appropriate semantic version, prefixed with a leading "v". For example,

```cmake
FetchContent_Declare(bronto
  GIT_REPOSITORY https://github.com/brontosource/bronto.git
  GIT_TAG        v17.34.51
)
```
