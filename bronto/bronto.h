#ifndef BRONTO_BRONTO_H
#define BRONTO_BRONTO_H

// `BRONTO_INLINE()`:
//
// A function-like macro that can be applied to function definitions (free or member)
// or to `using`-aliases to indicate that every usage of the
// annotated entity should be replaced with its definition. For example,
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
#define BRONTO_INTERNAL_REQUIRE_NO_ARGS()
#define BRONTO_INLINE(...)                                                     \
  BRONTO_INTERNAL_REQUIRE_NO_ARGS(__VA_ARGS__)                                 \
  __attribute__((annotate("bronto::inline_me")))

// `BRONTO_REFACTOR`:
//
// A compilation flag which is left undefined in most builds, but which the
// Bronto refactoring tool will specify as some nonzero integer literal.

// Though currently empty, this namespace will contain additional utilities for
// specifying refactorings.
namespace bronto {}  // namespace bronto

#endif  // BRONTO_BRONTO_H
