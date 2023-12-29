cmake_minimum_required(VERSION 3.10)

project(easy_tracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/accel/box.cpp
    src/accel/accel.cpp
    src/accel/accel_naive.cpp
    src/accel/accel_kdtree.cpp
    src/geometry/geometry.cpp
    src/geometry/triangle.cpp
    src/geometry/sphere.cpp
    src/tensor/vec4.cpp
    src/tensor/vec3.cpp
    src/tensor/vec2.cpp
    src/tensor/mat3.cpp
    src/texture/texture.cpp
    src/camera/camera.cpp
    src/camera/camera_pinhole.cpp
)

# Create a library target
add_library(easy_tracer_lib STATIC ${SOURCE_FILES})

# Set compile options for the library
target_compile_options(easy_tracer_lib PRIVATE -O3)

# Create executable targets and link with the library
add_executable(intersection example/intersection.cpp)
target_link_libraries(intersection PRIVATE easy_tracer_lib)

add_executable(local_coordinate example/local_coordinate.cpp)
target_link_libraries(local_coordinate PRIVATE easy_tracer_lib)

add_executable(sphere_test example/sphere_test.cpp)
target_link_libraries(sphere_test PRIVATE easy_tracer_lib)

add_executable(accel_test example/accel_test.cpp)
target_link_libraries(accel_test PRIVATE easy_tracer_lib)

add_executable(box_test example/box_test.cpp)
target_link_libraries(box_test PRIVATE easy_tracer_lib)

add_executable(texture_test example/texture_test.cpp)
target_link_libraries(texture_test PRIVATE easy_tracer_lib)

add_executable(recursive_texture example/recursive_texture.cpp)
target_link_libraries(recursive_texture PRIVATE easy_tracer_lib)
