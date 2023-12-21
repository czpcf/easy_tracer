cmake_minimum_required(VERSION 3.10)

project(easy_tracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    example/intersection.cpp
    src/accelerator/box.cpp
    src/geometry/triangle.cpp
    src/geometry/geometry.cpp
    src/tensor/vec3.cpp
    src/tensor/vec2.cpp
    src/tensor/mat3.cpp
)

add_executable(intersection ${SOURCE_FILES})

target_compile_options(intersection PRIVATE -O3)
