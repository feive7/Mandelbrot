cmake_minimum_required(VERSION 3.10)

project(mandelbrot)

find_package(raylib)
add_executable(mandelbrot main.cpp)
target_link_libraries(mandelbrot raylib)

install(TARGETS mandelbrot RUNTIME DESTINATION bin)
