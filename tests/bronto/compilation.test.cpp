// SPDX-License-Identifier: 0BSD

#include <bronto/bronto.hpp>

BRONTO_INLINE()
void f() {}

struct New {};
using Old BRONTO_INLINE() = New;

#if BRONTO_REFACTOR
#error This should never be executed because `BRONTO_REFACTOR` is not defined during compilation, only during the tool execution.
#endif

struct Rule : bronto::rewrite_decl {
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

int main() { return 0; }
