// SPDX-License-Identifier: 0BSD

#ifndef BRONTO_BRONTO_H
#define BRONTO_BRONTO_H

#define BRONTO_INTERNAL_REQUIRE_NO_ARGS()

// `BRONTO_INLINE()`:
//
// A function-like macro that can be applied to function definitions (free or
// member) or to `using`-aliases to indicate that every usage of the annotated
// entity should be replaced with its definition. For example,
//
// ```
// struct NewType {};
// using OldType BRONTO_INLINE() = NewType;
//
// BRONTO_INLINE()
// int zero() { return 0; }
// ```
//
// the code snippet above indicates that any use of `OldType` should be replaced
// with `NewType`, and that any call to the function `zero` should be replaced
// with the literal `0`.
//
// As needed for correctness, function arguments may be hoisted into local
// variables. Consider, for example
//
// ```
// BRONTO_INLINE()
// std::string duplicate(const std::string& s) { return s + s; }
//
// std::string some_string();
//
// void f() {
//   std::cout << duplicate(some_string());
// }
// ```
//
// Inside the function `f`, the call to `duplicate` will be inlined as
//
// ```
// void f() {
//   std::string s = some_string();
//   std::cout << s + s;
// }
// ```
//
// The `BRONTO_INLINE` function-like macro accepts exactly zero arguments.
// Though this could have been implemented with a non-function-like macro,
// requiring the parentheses allows for configuration arguments to be added
// without requiring a breaking change.
//
#define BRONTO_INLINE(...)                                                     \
  BRONTO_INTERNAL_REQUIRE_NO_ARGS(__VA_ARGS__)                                 \
  __attribute__((annotate("bronto::inline_me")))

// `BRONTO_BEFORE()`:
//
// A function-like macro that can be applied to functions inside a
// `bronto::rewrite_*` struct/class to indicate that the body of the function
// represents some pattern to be matched in the code. See
// https://brontosource.dev/docs for more details.
#define BRONTO_BEFORE()                                                        \
  BRONTO_INTERNAL_REQUIRE_NO_ARGS(__VA_ARGS__)                                 \
  __attribute__((annotate("bronto::before")))

// `BRONTO_AFTER()`:
//
// A function-like macro that can be applied to functions inside a
// `bronto::rewrite_*` struct/class to indicate that the body of the function
// represents some pattern to be used as a replacement in the code. See
// https://brontosource.dev/docs for more details.
#define BRONTO_AFTER()                                                         \
  BRONTO_INTERNAL_REQUIRE_NO_ARGS(__VA_ARGS__)                                 \
  __attribute__((annotate("bronto::after")))

namespace bronto {

/// `bronto::rewrite_expr`:
///
/// An empty struct used as a marker that any struct/class inheriting from it
/// represents an expression replacement. Member functions can be annotated as
/// `BRONTO_BEFORE` to indicate they represent a pattern to be searched for, or
/// `BRONTO_AFTER` to indicate they represent a replacement pattern.
struct rewrite_expr {};

}  // namespace bronto

// `BRONTO_REFACTOR`:
//
// A compilation flag which is left undefined in most builds, but which the
// Bronto refactoring tool will specify as some nonzero integer literal.

#endif  // BRONTO_BRONTO_H
