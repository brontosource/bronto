# SPDX-License-Identifier: 0BSD

add_executable(bronto.tests.compilation)
target_sources(bronto.tests.compilation PRIVATE compilation.test.cpp)
target_link_libraries(
    bronto.tests.compilation
    PRIVATE bronto
)

add_test(bronto.tests.compilation bronto.tests.compilation)
