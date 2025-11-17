// SPDX-License-Identifier: 0BSD

#include <bronto/bronto.hpp>

BRONTO_INLINE()
void f() {}

#if BRONTO_REFACTOR
#error This should never be executed because `BRONTO_REFACTOR` is not defined during compilation, only during the tool execution.
#endif

int main() { return 0; }
