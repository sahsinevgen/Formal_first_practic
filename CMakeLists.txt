cmake_minimum_required(VERSION 3.6)

project(cpp-json-example-tests)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS "--coverage -fprofile-arcs -ftest-coverage -lgcov")

include_directories(
    ${GTEST_INCLUDE_DIRS}
)

add_subdirectory(libs)

add_executable(main ./main.cpp)
target_link_libraries(main libs)

