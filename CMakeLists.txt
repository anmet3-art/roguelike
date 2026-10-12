cmake_minimum_required(VERSION 3.20)
project(roguelike)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(roguelike src/main.cpp)
target_compile_options(roguelike PRIVATE -Wall -Wextra)

