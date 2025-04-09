# SPDX-License-Identifier: 0BSD

add_library(bronto)

target_sources(bronto PRIVATE bronto.cpp)

target_sources(
    bronto
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
        FILES ${PROJECT_SOURCE_DIR}/include/bronto/bronto.hpp
)

set_target_properties(
    bronto
    PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON EXPORT_NAME bronto
)

install(
    TARGETS bronto
    EXPORT bronto
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}$<$<CONFIG:Debug>:/debug>
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}$<$<CONFIG:Debug>:/debug>
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
