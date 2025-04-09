# SPDX-License-Identifier: 0BSD

cmake_minimum_required(VERSION 3.10.0)

project("bronto"
        VERSION 0.1.0
        DESCRIPTION "Support library for describing refactorings with the Bronto Refactoring Tool"
        LANGUAGES CXX
        HOMEPAGE_URL "https://brontosource.dev")

option(
    BRONTO_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

add_subdirectory(src/bronto)

if(BRONTO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/bronto)
endif()
