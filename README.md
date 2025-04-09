# Bronto

This repository holds infrastructure to help automate refactoring and migrations
of C++ code. The library contains annotations that can be added to your existing
C++ code to indicate transformations which you desire to happen in the
background. The Bronto refactoring tool (not included here) is responsible for
understanding these annotations and producing the corresponding changes. If this
interests you, please contact us at [contact@brontosource.dev](
mailto:contact@brontosource.dev).

The infrastructure included in this repository is a single header file. It is
designed to have no run-time cost and an immeasurably small compile-time
footprint.
