// SPDX-License-Identifier: 0BSD

#ifndef BRONTO_BRONTO_H
#define BRONTO_BRONTO_H

#define BRONTO_INTERNAL_CAT(a, b) BRONTO_INTERNAL_CAT_(a, b)
#define BRONTO_INTERNAL_CAT_(a, b) a##b

#define BRONTO_INTERNAL_STRINGIFY(s) BRONTO_INTERNAL_STRINGIFY_(s)
#define BRONTO_INTERNAL_STRINGIFY_(s) #s

#ifdef __clang__

#define BRONTO_INTERNAL_PUSH_WARNING_STATE _Pragma("GCC diagnostic push")
#define BRONTO_INTERNAL_POP_WARNING_STATE _Pragma("GCC diagnostic pop")

#define BRONTO_INTERNAL_ERROR_IGNORED_ATTRIBUTES                               \
  _Pragma("GCC diagnostic error \"-Wignored-attributes\"")                     \
      _Pragma("GCC diagnostic ignored \"-Wunknown-attributes\"")               \
          _Pragma("GCC diagnostic ignored \"-Wmissing-declarations\"")

#else

#define BRONTO_INTERNAL_PUSH_WARNING_STATE
#define BRONTO_INTERNAL_POP_WARNING_STATE

#define BRONTO_INTERNAL_ERROR_IGNORED_ATTRIBUTES

#endif

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
#ifdef BRONTO_REFACTOR
#define BRONTO_INLINE() [[clang::annotate("bronto::inline_me")]]
#else
#define BRONTO_INLINE()
#endif

// `BRONTO_BEFORE()`:
//
// A function-like macro that can be applied to functions inside a
// `bronto::rewrite_*` struct/class to indicate that the body of the function
// represents some pattern to be matched in the code. See
// https://brontosource.dev/docs for more details.
#ifdef BRONTO_REFACTOR
#define BRONTO_BEFORE() [[clang::annotate("bronto::before")]]
#else
#define BRONTO_BEFORE()
#endif

// `BRONTO_AFTER()`:
//
// A function-like macro that can be applied to functions inside a
// `bronto::rewrite_*` struct/class to indicate that the body of the function
// represents some pattern to be used as a replacement in the code. See
// https://brontosource.dev/docs for more details.
#ifdef BRONTO_REFACTOR
#define BRONTO_AFTER() [[clang::annotate("bronto::after")]]
#else
#define BRONTO_AFTER()
#endif

// `BRONTO_USAGE(tag)`:
//
// A function-like macro that can be applied to a `bronto::rewrite_*`
// struct/class nested inside a struct/class inheriting from
// `bronto::rewrite_decl`. Indicates that in order for the rewrite to be applied
// to that declaration, some requirements are placed on the uses of the declared
// variable. Specifically,
//
// * `BRONTO_USAGE(required)`: Requires that at least one use of the declared
//   variable satisfies the constraints specified by the rule for the rewrite to
//   occur. If the rewrite does occur, any matching usage will be rewritten
//   according to this rule.
//
// * `BRONTO_USAGE(allowed)`: Makes no requirements about the usage of the
//   declared variable, but rewrites each usage that satisfies the constraints
//   of the rule.
//
// * `BRONTO_USAGE(forbidden)`: Requires that no use of the declared variable
//   may satisfiy the constraints specified by the rule for the rewrite to
//   occur.
//
// If multiple rules have a `BRONTO_USAGE(tag)` attribute, all constraints are
// required. See https://brontosource.dev/docs for more details.
#define BRONTO_USAGE(tag)                                                      \
  BRONTO_INTERNAL_VALIDATE_USAGE(BRONTO_INTERNAL_CAT(                          \
      BRONTO_INTERNAL_USAGE_KIND_, tag)())(tag) BRONTO_INTERNAL_USAGE(tag)

#define BRONTO_INTERNAL_VALIDATE_USAGE(...)                                    \
  BRONTO_INTERNAL_VALIDATE_USAGE_(__VA_ARGS__, BRONTO_INTERNAL_USAGE_OKAY,     \
                                  BRONTO_INTERNAL_USAGE_FAIL)
#define BRONTO_INTERNAL_VALIDATE_USAGE_(...)                                   \
  BRONTO_INTERNAL_VALIDATE_USAGE_GET(__VA_ARGS__)
#define BRONTO_INTERNAL_VALIDATE_USAGE_GET(arg0, arg1, N, ...) N

// Anything with exactly two arguments suffices
#define BRONTO_INTERNAL_ACCEPTABLE_USAGE 0, 1
#define BRONTO_INTERNAL_USAGE_KIND_required() BRONTO_INTERNAL_ACCEPTABLE_USAGE
#define BRONTO_INTERNAL_USAGE_KIND_allowed() BRONTO_INTERNAL_ACCEPTABLE_USAGE
#define BRONTO_INTERNAL_USAGE_KIND_forbidden() BRONTO_INTERNAL_ACCEPTABLE_USAGE

// clang-format off
#define BRONTO_INTERNAL_USAGE_FAIL(tag) \
    BrontoInternalIgnoreStruct; static_assert(false, R"(
  The tag ")" BRONTO_INTERNAL_STRINGIFY(tag) R"(" is not supported inside the BRONTO_USAGE macro.
  Only "required", "allowed", and "forbidden" are supported.
  See https://brontosource.dev/docs for more documentation.

)"); struct
// clang-format on

#ifdef BRONTO_REFACTOR
#define BRONTO_INTERNAL_USAGE(tag)                                             \
  BRONTO_INTERNAL_PUSH_WARNING_STATE                                           \
  BRONTO_INTERNAL_ERROR_IGNORED_ATTRIBUTES                                     \
  [[clang::annotate("bronto::usage::" BRONTO_INTERNAL_STRINGIFY(               \
      tag))]] BRONTO_INTERNAL_PUSH_WARNING_STATE

#define BRONTO_INTERNAL_USAGE_OKAY(tag)
#else
#define BRONTO_INTERNAL_USAGE(tag)

#define BRONTO_INTERNAL_USAGE_OKAY(tag)                                        \
  BRONTO_INTERNAL_PUSH_WARNING_STATE                                           \
  BRONTO_INTERNAL_ERROR_IGNORED_ATTRIBUTES                                     \
  [[maybe_unused]] BRONTO_INTERNAL_PUSH_WARNING_STATE

#endif

namespace bronto {

// `bronto::rewrite_decl`:
//
// An empty struct used as a marker that any struct/class inheriting from it
// represents a declaration replacement. Member functions can be annotated as
// `BRONTO_BEFORE` to indicate they represent a pattern to be searched for, or
// `BRONTO_AFTER` to indicate they represent a replacement pattern. See
// https://brontosource.dev/docs for more details.
struct rewrite_decl {};

// `bronto::rewrite_expr`:
//
// An empty struct used as a marker that any struct/class inheriting from it
// represents an expression replacement. Member functions can be annotated as
// `BRONTO_BEFORE` to indicate they represent a pattern to be searched for, or
// `BRONTO_AFTER` to indicate they represent a replacement pattern. See
// https://brontosource.dev/docs for more details.
struct rewrite_expr {};

}  // namespace bronto

// `BRONTO_REFACTOR`:
//
// A compilation flag which is left undefined in most builds, but which the
// Bronto refactoring tool will specify as some nonzero integer literal.

#endif  // BRONTO_BRONTO_H
