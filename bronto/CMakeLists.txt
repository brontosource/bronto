add_library(bronto INTERFACE)
target_include_directories(bronto INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_features(bronto INTERFACE cxx_std_17)

add_executable(bronto_test bronto_compilation_test_source.cc)
target_link_libraries(bronto_test PRIVATE bronto)
