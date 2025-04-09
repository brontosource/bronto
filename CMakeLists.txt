cmake_minimum_required(VERSION 3.10.0)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" PROJECT_VERSION_RAW)
string(STRIP "${PROJECT_VERSION_RAW}" PROJECT_VERSION)

project("bronto"
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX
        DESCRIPTION "Support library for describing refactorings with the Bronto Refactoring Tool"
        HOMEPAGE_URL "https://brontosource.dev")

add_subdirectory(bronto)
