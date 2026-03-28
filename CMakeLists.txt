cmake_minimum_required(VERSION 3.10)
project(TUCIL2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(glad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

add_executable(main
    src/main.cpp
    src/dnc.cpp
    src/octree.cpp
    src/io.cpp
    src/viewer/viewer.cpp
)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/viewer
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

target_link_libraries(main PRIVATE glad glfw OpenGL::GL)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/VertexShader.vert
    ${CMAKE_BINARY_DIR}/VertexShader.vert
    COPYONLY
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/FragmentShader.frag
    ${CMAKE_BINARY_DIR}/FragmentShader.frag
    COPYONLY
)