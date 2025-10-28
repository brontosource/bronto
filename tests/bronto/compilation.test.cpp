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

int main() { return 0; }
