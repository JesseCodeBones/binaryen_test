cmake_minimum_required(VERSION 3.0.0)
project(binaryen_test VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
include(CTest)
enable_testing()
set(BUILD_TESTS 0)
add_executable(binaryen_test main.cpp)
add_subdirectory(binaryen)
target_link_libraries(binaryen_test binaryen)
target_include_directories(binaryen_test PRIVATE ./binaryen/src )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
