// SPDX-License-Identifier: 0BSD

#include <bronto/bronto.hpp>
#include <cstdint>

BRONTO_INLINE()
void f() {}

struct New {};
using Old BRONTO_INLINE() = New;

#if BRONTO_REFACTOR
#error This should never be executed because `BRONTO_REFACTOR` is not defined during compilation, only during the tool execution.
#endif

struct DeclRule : bronto::rewrite_decl {
  struct BRONTO_USAGE(required) Required : bronto::rewrite_expr {};
  struct BRONTO_USAGE(allowed) Allowed : bronto::rewrite_expr {};
  struct BRONTO_USAGE(forbidden) Forbidden : bronto::rewrite_expr {};
};

struct RuleDefault : bronto::rewrite_decl {
  BRONTO_BEFORE()
  void before() { int var; }

  BRONTO_AFTER()
  void after() { int var; }
};

struct RuleStrict : bronto::rewrite_decl {
  BRONTO_BEFORE("strict")
  void before() { int var; }

  BRONTO_AFTER("strict")
  void after() { int var; }
};

struct RuleLoose : bronto::rewrite_decl {
  BRONTO_BEFORE("loose")
  void before() { int var; }

  BRONTO_AFTER("loose")
  void after() { int var; }
};

struct TypeRule : bronto::rewrite_type {
  using before BRONTO_BEFORE() = std::int32_t;
  using after BRONTO_AFTER()   = std::int64_t;
};

struct ExprRule : bronto::rewrite_expr {
  BRONTO_BEFORE()
  int before(int x BRONTO_BINDS(bronto::literal)) { return x * 2; }
};

#if __cplusplus >= 201103L

#include <type_traits>
#include <utility>

// The two-argument `bronto::eval(value, tag)` overload accepts `value` only
// when it is a non-class type or a class exposing a char-pointer `data()` and a
// `size()`. These checks exercise that SFINAE gate in unevaluated contexts.
namespace eval_test {

template <typename T>
T&& declval();

struct Kb {};
// Our SFINAE check doesn't verify that the operator accepts the type in the
// first argument. The validation for that is done within the tool, so this
// operator's signature doesn't matter.
Kb operator""_kb(unsigned long long);

// True iff `bronto::eval(value, 0_kb)` is well-formed for a value of type `T`.
template <typename T>
class is_udl_evaluable {
  template <typename U>
  static auto test(int) -> decltype(bronto::eval(declval<U>(), 0_kb), char());
  template <typename U>
  static long test(...);

 public:
  enum : bool { value = sizeof(test<T>(0)) == sizeof(char) };
};

// Accepted: non-class types (integers, floating point, characters, pointers).
static_assert(is_udl_evaluable<int>::value, "int is evaluable");
static_assert(is_udl_evaluable<double>::value, "double is evaluable");
static_assert(is_udl_evaluable<char>::value, "char is evaluable");
static_assert(is_udl_evaluable<char const*>::value,
              "char pointer is evaluable");

// Accepted: a class exposing a char-pointer `data()` and a `size()`.
struct StringLike {
  char const* data() const;
  unsigned long size() const;
};
static_assert(is_udl_evaluable<StringLike>::value,
              "class with char-pointer data() and size() is evaluable");

// Rejected: a class with `size()` but a non-char-pointer `data()`.
struct IntData {
  int const* data() const;
  unsigned long size() const;
};
static_assert(not is_udl_evaluable<IntData>::value,
              "class with non-char-pointer data() is not evaluable");

// Rejected: a class with `data()` but no `size()`.
struct NoSize {
  char const* data() const;
};
static_assert(not is_udl_evaluable<NoSize>::value,
              "class with data() but no size() is not evaluable");

// Rejected: a class exposing neither `data()` nor `size()`.
struct Plain {};
static_assert(not is_udl_evaluable<Plain>::value,
              "plain class is not evaluable");

}  // namespace eval_test

// `bronto::include` accepts a `bronto::HeaderInclude<String>` only when
// `String` is a string-like or character-pointer type with `char` code units.
// These checks exercise that SFINAE gate in unevaluated contexts.
namespace include_test {

// True iff `bronto::include(h)` is well-formed for `h` of type
// `bronto::HeaderInclude<T>`.
template <typename T>
class is_includable {
  template <typename U>
  static auto test(int)
      -> decltype(bronto::include(std::declval<bronto::HeaderInclude<U>>()),
                  char());
  template <typename U>
  static long test(...);

 public:
  enum : bool { value = sizeof(test<T>(0)) == sizeof(char) };
};

// Accepted: pointers to `char`.
static_assert(is_includable<char const*>::value, "char pointer is includable");
static_assert(is_includable<char*>::value,
              "mutable char pointer is includable");

// Accepted: a class exposing a `char` pointer `data()` and a `size()`.
static_assert(is_includable<eval_test::StringLike>::value,
              "class with char-pointer data() and size() is includable");

// Rejected: strings of any other code unit type.
static_assert(not is_includable<wchar_t const*>::value,
              "wide char pointer is not includable");
static_assert(not is_includable<char16_t const*>::value,
              "char16_t pointer is not includable");

struct WideStringLike {
  wchar_t const* data() const;
  unsigned long size() const;
};
static_assert(not is_includable<WideStringLike>::value,
              "class with wide char-pointer data() is not includable");

// Rejected: types that are not strings at all.
static_assert(not is_includable<int>::value, "int is not includable");
static_assert(not is_includable<eval_test::IntData>::value,
              "class with non-char-pointer data() is not includable");
static_assert(not is_includable<eval_test::NoSize>::value,
              "class with data() but no size() is not includable");
static_assert(not is_includable<eval_test::Plain>::value,
              "plain class is not includable");

// A braced list initializes a `bronto::HeaderInclude<char const*>`.
static_assert(
    std::is_same<decltype(bronto::include({"vector", true})), void>::value,
    "a braced list is includable");

// A replacement requesting headers, written as it would be in a rule.
struct IncludeRule : bronto::rewrite_expr {
  BRONTO_BEFORE()
  int before(int x) { return x; }

  BRONTO_AFTER()
  int after(int x) {
    bronto::include(bronto::HeaderInclude<char const*>{"vector", true});
    bronto::include(
        bronto::HeaderInclude<char const*>{"project/header.h", false});
    return x;
  }
};

// The same requests, written as braced lists.
struct BracedIncludeRule : bronto::rewrite_expr {
  BRONTO_BEFORE()
  int before(int x) { return x; }

  BRONTO_AFTER()
  int after(int x) {
    bronto::include({"vector", true});
    bronto::include({"project/header.h", false});
    return x;
  }
};

#if defined(__cpp_deduction_guides)

// A string literal deduces `char const*`, since the guide takes it by value.
static_assert(std::is_same<decltype(bronto::HeaderInclude{"vector", true}),
                           bronto::HeaderInclude<char const*>>::value,
              "a string literal deduces char const*");
static_assert(std::is_same<decltype(bronto::HeaderInclude{
                               eval_test::StringLike(), false}),
                           bronto::HeaderInclude<eval_test::StringLike>>::value,
              "a string-like class deduces itself");

// The same requests, with the template argument deduced.
struct DeducedIncludeRule : bronto::rewrite_expr {
  BRONTO_BEFORE()
  int before(int x) { return x; }

  BRONTO_AFTER()
  int after(int x) {
    bronto::include(bronto::HeaderInclude{"vector", true});
    bronto::include(bronto::HeaderInclude{"project/header.h", false});
    return x;
  }
};

#endif  // defined(__cpp_deduction_guides)

}  // namespace include_test

#endif  // __cplusplus >= 201103L

int main() { return 0; }
