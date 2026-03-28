cmake_minimum_required(VERSION 3.10)
project(TUCIL2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main
    src/main.cpp
    src/dnc.cpp
    src/octree.cpp
    src/io.cpp
)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)