cmake_minimum_required(VERSION 3.10)

project(easy_tracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/accel/box.cpp
    src/geometry/geometry.cpp
    src/geometry/triangle.cpp
    src/geometry/sphere.cpp
    src/tensor/vec3.cpp
    src/tensor/vec2.cpp
    src/tensor/mat3.cpp
)

add_executable(intersection example/intersection.cpp ${SOURCE_FILES})
target_compile_options(intersection PRIVATE -O3)

add_executable(local_coordinate example/local_coordinate.cpp ${SOURCE_FILES})
target_compile_options(local_coordinate PRIVATE -O3)

add_executable(sphere_test example/sphere_test.cpp ${SOURCE_FILES})
target_compile_options(sphere_test PRIVATE -O3)
