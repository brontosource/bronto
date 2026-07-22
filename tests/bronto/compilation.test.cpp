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

#if __cplusplus >= 201103L

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

#endif  // __cplusplus >= 201103L

int main() { return 0; }
