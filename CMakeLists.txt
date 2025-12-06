cmake_minimum_required(VERSION 3.16)
project(jadidi)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

add_executable(jadidi main.cpp)

target_link_libraries(jadidi PRIVATE SDL2::SDL2 SDL2::SDL2main)