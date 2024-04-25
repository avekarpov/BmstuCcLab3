cmake_minimum_required(VERSION 3.20.0)
project(lab3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC "src/*.cpp")

add_executable(lab1 ${SRC})

target_include_directories(
    lab1
    PRIVATE
        include
)

add_subdirectory(test)
