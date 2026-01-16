# SPDX-License-Identifier: 0BSD

add_executable(bronto.tests.compilation)
target_sources(bronto.tests.compilation PRIVATE compilation.test.cpp)
target_link_libraries(
    bronto.tests.compilation
    PRIVATE bronto
)

add_test(bronto.tests.compilation bronto.tests.compilation)

add_executable(bronto.tests.legacy)
target_sources(bronto.tests.legacy PRIVATE compilation.test.cpp)
target_link_libraries(
    bronto.tests.legacy
    PRIVATE bronto
)
set_target_properties(bronto.tests.legacy PROPERTIES
  CXX_STANDARD 98
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)
add_test(bronto.tests.legacy bronto.tests.legacy)
