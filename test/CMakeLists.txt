cmake_minimum_required(VERSION 3.20.0)
project(test LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 20)

add_subdirectory(libs)

file(GLOB_RECURSE test_SRC "src/*.cpp")

add_executable(test ${test_SRC})

target_include_directories(
    test
    PRIVATE
        ../include
)

target_link_libraries(
    test
    PUBLIC
        Catch2::Catch2WithMain
)
