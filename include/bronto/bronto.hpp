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

// `BRONTO_EXTRACT()`:
//
// A function-like macro that can be applied to function definitions to indicate
// that every expression matching the single statement in the annotated
// function's body should be replaced with a call to that function. For example,
//
// ```
// BRONTO_EXTRACT()
// std::string duplicate(const std::string& s) { return s + s; }
//
// std::string x, y;
//
// void f() {
//   std::cout << x + x;
//   std::cout << y + y;
//   std::cout << x + y;
// }
// ```
//
// The code snippet above indicates that any expression of the form `e + e`
// where `e` is a `std::string` should be replaced with a call to `duplicate`,
// transforming `f` into
//
// ```
// void f() {
//   std::cout << duplicate(x);
//   std::cout << duplicate(y);
//   std::cout << x + y;
// }
// ```
//
// The `BRONTO_EXTRACT` function-like macro accepts exactly zero arguments.
// Though this could have been implemented with a non-function-like macro,
// requiring the parentheses allows for configuration arguments to be added
// without requiring a breaking change.
//
#ifdef BRONTO_REFACTOR
#define BRONTO_EXTRACT() [[clang::annotate("bronto::extract_me")]]
#else
#define BRONTO_EXTRACT()
#endif

// `BRONTO_BEFORE()`:
//
// A function-like macro that can be applied to functions inside a
// `bronto::rewrite_*` struct/class to indicate that the body of the function
// represents some pattern to be matched in the code. See
// https://brontosource.dev/docs for more details.
#ifdef BRONTO_REFACTOR
#define BRONTO_BEFORE(...)                                                     \
  [[clang::annotate("bronto::before", BRONTO_INTERNAL_CONFIG(__VA_ARGS__))]]
#else
#define BRONTO_BEFORE(...)
#endif

// `BRONTO_AFTER()`:
//
// A function-like macro that can be applied to functions inside a
// `bronto::rewrite_*` struct/class to indicate that the body of the function
// represents some pattern to be used as a replacement in the code. See
// https://brontosource.dev/docs for more details.
#ifdef BRONTO_REFACTOR
#define BRONTO_AFTER(...)                                                      \
  [[clang::annotate("bronto::after", BRONTO_INTERNAL_CONFIG(__VA_ARGS__))]]
#else
#define BRONTO_AFTER(...)
#endif

// `BRONTO_USAGE(tag)`:
//
// A function-like macro that can be applied to a `bronto::rewrite_*`
// struct/class nested inside a struct/class inheriting from
// `bronto::rewrite_decl` or `bronto::rewrite_param`. Indicates that in order
// for the rewrite to be applied to that declaration, some requirements are
// placed on the uses of the declared variable. Specifically,
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
#define BRONTO_INTERNAL_USAGE_FAIL_MESSAGE(tag) "\n" \
"  The tag \"" BRONTO_INTERNAL_STRINGIFY(tag) "\" is not supported inside the BRONTO_USAGE macro.\n" \
"  Only \"required\", \"allowed\", and \"forbidden\" are supported.\n" \
"  See https://brontosource.dev/docs for more documentation.\n\n"
// clang-format on

#if __clang__ || __GNUC__
#define BRONTO_INTERNAL_USAGE_FAIL(tag)                                        \
  BrontoInternalIgnoreStruct;                                                  \
  _Pragma(BRONTO_INTERNAL_STRINGIFY(                                           \
      GCC error BRONTO_INTERNAL_USAGE_FAIL_MESSAGE(tag))) struct
#elif __cpp_static_assert >= 200410L
#define BRONTO_INTERNAL_USAGE_FAIL(tag)                                        \
  BrontoInternalIgnoreStruct;                                                  \
  static_assert(false, BRONTO_INTERNAL_USAGE_FAIL_MESSAGE(tag));               \
  struct
#endif

#ifdef BRONTO_REFACTOR
#define BRONTO_INTERNAL_USAGE(tag)                                             \
  BRONTO_INTERNAL_PUSH_WARNING_STATE                                           \
  BRONTO_INTERNAL_ERROR_IGNORED_ATTRIBUTES                                     \
  [[clang::annotate("bronto::usage::" BRONTO_INTERNAL_STRINGIFY(               \
      tag))]] BRONTO_INTERNAL_POP_WARNING_STATE

#define BRONTO_INTERNAL_USAGE_OKAY(tag)
#else
#define BRONTO_INTERNAL_USAGE(tag)

#define BRONTO_INTERNAL_USAGE_OKAY(tag)                                        \
  BRONTO_INTERNAL_PUSH_WARNING_STATE                                           \
  BRONTO_INTERNAL_ERROR_IGNORED_ATTRIBUTES                                     \
  [[maybe_unused]] BRONTO_INTERNAL_POP_WARNING_STATE

#endif

// `BRONTO_INLINE_MACRO()`:
//
// An empty function-like macro that can be applied to macro definitions as the
// first tokens, indicating that expansions of that macro should be replaced
// with the remaining tokens. For example,
//
// ```
// #define MY_MACRO BRONTO_INLINE_MACRO() (1 + 5)
//
// int six() { return MY_MACRO; }
// ```
//
// the code snippet above indicates that any use of `MY_MACRO` should be
// replaced with `(1 + 5)`.
//
// Function-like macros can also be inlined, with their arguments replacing
// paramters in the resulting text, such as:
//
// ```
// #define _min(a, b) BRONTO_INLINE_MACRO() std::min(a, b)
//
// int zero() { return _min(1, 0); }
// ```
//
// the snippet above will be tranformed into:
//
// ```
// int zero() { return std::min(1, 0); }
// ```
//
// The `BRONTO_INLINE_MACRO` function-like macro accepts exactly zero arguments.
// Though this could have been implemented with a non-function-like macro,
// requiring the parentheses allows for configuration arguments to be added
// without requiring a breaking change.
//
#define BRONTO_INLINE_MACRO()

#ifdef __cplusplus
namespace bronto {
namespace internal {

#ifdef __cpp_constexpr
inline constexpr int config(...) { return 0; }
#define BRONTO_INTERNAL_CONFIG(...) ::bronto::internal::config(__VA_ARGS__)
#else
#define BRONTO_INTERNAL_CONFIG(...)                                            \
  BRONTO_INTERNAL_EXPAND(__VA_ARGS__ __VA_OPT__(, ) BRONTO_INTERNAL_CONFIG_1,  \
                         BRONTO_INTERNAL_CONFIG_0)(__VA_ARGS__)
#define BRONTO_INTERNAL_EXPAND(_, head, ...) head
#define BRONTO_INTERNAL_CONFIG_0() "loose"
#define BRONTO_INTERNAL_CONFIG_1(config) config
#endif

}  // namespace internal

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

// `bronto::rewrite_param`:
//
// An empty struct used as a marker that any struct/class inheriting from it
// represents a parameter replacement. Member functions can be annotated as
// `BRONTO_BEFORE` to indicate they represent a pattern to be searched for, or
// `BRONTO_AFTER` to indicate they represent a replacement pattern. See
// https://brontosource.dev/docs for more details.
struct rewrite_param {};

// `bronto::rewrite_type`:
//
// An empty struct used as a marker that any struct/class inheriting from it
// represents a type replacement. Member aliases and alias templates can be
// annotated as `BRONTO_BEFORE` to indicate they represent a pattern to be
// searched for, or `BRONTO_AFTER` to indicate they represent a replacement
// pattern. See https://brontosource.dev/docs for more details.
struct rewrite_type {};

// `bronto::rewrite_lambda_capture`:
//
// An empty base class marker used to indicate that a struct represents a lambda
// capture replacement. Member functions can be annotated as `BRONTO_BEFORE` to
// indicate that they represent a pattern to be searched for or `BRONTO_AFTER`
// to indicate they represent a replacement pattern. See
// https://brontosource.dev/docs for more details.
struct rewrite_lambda_capture {};

// `bronto::avoid_adl`:
//
// An empty class that can be inherited from to bring in other namespaces for
// the purposes of argument-dependent-lookup, and to declare that functions
// found via this type for argument-dependent-lookup should be replaced with
// qualified function calls. Function call will be rewritten minimally. That
// is, a function will be rewritten only when the argument whose type inherits
// from `avoid_adl` is the only reason the function declaration was found.
// See https://brontosource.dev/docs for more details.
template <typename, typename...>
struct avoid_adl {};

#if __cplusplus >= 201103L

namespace internal {

template <typename T>
T&& declval();

template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
  typedef T type;
};

template <typename...>
struct voider {
  typedef void type;
};

template <typename C>
struct char_pointer_base {
  typedef C type;
};

template <typename P>
struct char_pointer {};

// clang-format off
template <> struct char_pointer<char*>           : char_pointer_base<char>     {};
template <> struct char_pointer<char const*>     : char_pointer_base<char>     {};
template <> struct char_pointer<wchar_t*>        : char_pointer_base<wchar_t>  {};
template <> struct char_pointer<wchar_t const*>  : char_pointer_base<wchar_t>  {};
template <> struct char_pointer<char16_t*>       : char_pointer_base<char16_t> {};
template <> struct char_pointer<char16_t const*> : char_pointer_base<char16_t> {};
template <> struct char_pointer<char32_t*>       : char_pointer_base<char32_t> {};
template <> struct char_pointer<char32_t const*> : char_pointer_base<char32_t> {};
#if defined(__cpp_char8_t)
template <> struct char_pointer<char8_t*>        : char_pointer_base<char8_t>  {};
template <> struct char_pointer<char8_t const*>  : char_pointer_base<char8_t>  {};
#endif
// clang-format on

template <typename T, typename = void>
struct is_char_pointer {
  enum : bool { value = false };
};

template <typename T>
struct is_char_pointer<T,
                       typename voider<typename char_pointer<T>::type>::type> {
  enum : bool { value = true };
};

template <typename T, typename = void>
struct is_string_like {
  enum : bool { value = false };
};

template <typename T>
struct is_string_like<
    T,
    typename voider<typename char_pointer<decltype(declval<T&>().data())>::type,
                    decltype(declval<T&>().size())>::type> {
  enum : bool { value = true };
};

template <typename T>
struct string_literal_type {
  typedef
      typename char_pointer<decltype(declval<T&>().data())>::type const* type;
};

template <typename T>
struct is_class_type {
  template <typename U>
  static char test(int U::*);
  template <typename U>
  static long test(...);

  enum : bool { value = sizeof(test<T>(0)) == sizeof(char) };
};

template <typename T>
struct is_evaluable {
  enum : bool {
    value = not is_class_type<T>::value or is_string_like<T>::value
  };
};

}  // namespace internal

// `bronto::eval`:
//
// In a replacement, `bronto::eval(expr)` constant-evaluates `expr` and uses the
// result as replacement. For example, `bronto::eval(1 + 2)` writes the value
// `3` rather than the spliced text `1 + 2`. Parameters of the replacement
// function may appear in `expr`, and the expressions captured at the rewrite
// site are substituted for them before evaluation.
//
// `bronto::eval` may only appear in a function annotated with `BRONTO_AFTER()`.
// If the substitution fails, or the substituted expression cannot be
// constant-evaluated, the matched site is left unchanged and a diagnostic
// explains why.
//
// The literal denotes the evaluated value exactly. The type of the expression
// determines the kind of literal generated:
//
//   Expression type       Generated literal            Example
//   --------------------  ---------------------------  ------------------------
//   bool                  boolean literal              true
//   int                   integer, no suffix           42
//   unsigned              integer, u suffix            42u
//   long                  integer, l suffix            42l
//   unsigned long         integer, ul suffix           42ul
//   long long             integer, ll suffix           42ll
//   unsigned long long    integer, ull suffix          42ull
//   signed char,          static_cast of an            static_cast<short>(1000)
//     unsigned char,        integer literal
//     short,
//     unsigned short
//   char                  character, no prefix         'a'
//   wchar_t               character, L prefix          L'a'
//   char8_t               character, u8 prefix         u8'a'
//   char16_t              character, u prefix          u'a'
//   char32_t              character, U prefix          U'a'
//   float                 floating-point, f suffix     1.5f
//   double                floating-point, no suffix    1.5
//   long double           floating-point, L suffix     1.5L
//   string-like,          string, prefix matching      "abc", L"abc"
//     character pointer     the code unit type
//
// For character results, printable ASCII characters are written verbatim,
// mnemonic escapes (e.g. `\n`) are used where they exist portably, and any
// other value uses a numeric escape. Note that `signed char` and `unsigned
// char` are treated as integer types, not character types.
//
// For floating-point results, the expression is constant-evaluated with IEEE
// 754 arithmetic in the result type's format on the target (as specified in
// your compile_commands.json; e.g. `long double` varies by target). The
// following applies:
//
//   - Values are rounded to nearest with ties to even.
//   - Operations are never contracted. For example, `a * b + c` is evaluated as
//     a rounded multiply then a rounded add, never a fused multiply-add, so the
//     result may differ from what the same expression would compute at runtime
//     under flags like `-ffp-contract=fast`.
//   - The emitted literal denotes the evaluated value exactly. Any compiler
//     that parses floating-point literals with correct rounding reproduces the
//     value bit-for-bit. It is not necessarily the shortest spelling of that
//     value.
//
// To output a string literal, the evaluated expression must be of a type that
// has a constexpr `data()` member function returning a pointer to a standard
// character type and a constexpr `size()` member function returning its length.
// In C++17 `std::string_view` satisfies these requirements, and in C++20
// `std::string` does as well. The literal denotes each code unit of the result
// exactly, using the same escaping rules as character literals.
//
// An expression whose type is itself a pointer to a standard character type
// (e.g. `char const*`) is accepted as a NUL-terminated string. A null pointer
// or a character array without a terminator fails to constant-evaluate.
//
// For a string-like or character-pointer expression, `bronto::eval(expr)` is
// declared to return a pointer to the constant character type (e.g.
// `char const*`) rather than the expression's own type. This models the
// string literal that will be generated in the replacement text.
template <typename T, typename internal::enable_if<
                          not internal::is_string_like<T>::value and
                              not internal::is_char_pointer<T>::value,
                          int>::type = 0>
T eval(T value);

template <typename T, typename internal::enable_if<
                          internal::is_string_like<T>::value, int>::type = 0>
typename internal::string_literal_type<T>::type eval(T value);

template <typename T, typename internal::enable_if<
                          internal::is_char_pointer<T>::value, int>::type = 0>
typename internal::char_pointer<T>::type const* eval(T value);

// The two-argument `bronto::eval` emits its value as a UDL (user-defined
// literal).
//
// `bronto::eval(value, tag)` constant-evaluates `value` and writes it as a
// UDL carrying `tag`'s suffix and prefix. `tag` is a UDL token whose content is
// ignored. It names the literal operator, the token category, and the prefix.
// For example, given an operator `Bytes operator""_kb(unsigned long long)`, a
// replacement written
//
//     bronto::eval(n, 0_kb);
//
// writes `512_kb` when `n` evaluates to 512. The operator is never evaluated by
// the tool, so it need not be constexpr. The following operator forms are
// accepted:
//
//   - X operator""_x(unsigned long long): e.g. 100_x
//   - X operator""_x(long double): e.g. 100.0_x
//   - X operator""_x(CharT const*, size_t): e.g. u"100"_x
//   - X operator""_x(CharT): e.g. u'1'_x
//
// where `CharT` is a standard character type.
//
// The following forms are _not_ accepted:
//
//   - X operator""_x(const char*): Raw literal operator
//   - template <char...> X operator""_x(): Numeric literal operator template
//   - template <T t> X operator""_x(): String literal operator template
//
// A string is the only literal a class type can produce, so `value` may only
// have a class type if it exposes a constexpr `data()` and `size()`.
template <int&... ExplicitArgumentBarrier, typename T, typename R,
          typename internal::enable_if<internal::is_evaluable<T>::value,
                                       int>::type = 0>
R eval(T value, R&& tag);

#endif  // __cplusplus >= 201103L

}  // namespace bronto
#endif  // __cplusplus

// `BRONTO_REFACTOR`:
//
// A compilation flag which is left undefined in most builds, but which the
// Bronto refactoring tool will specify as some nonzero integer literal.

#endif  // BRONTO_BRONTO_H
