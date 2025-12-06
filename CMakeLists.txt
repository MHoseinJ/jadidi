cmake_minimum_required(VERSION 3.16)
project(jadidi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE ENGINE_SOURCES "engine/*.cpp")

add_executable(jadidi
        main.cpp
        ${ENGINE_SOURCES}
)

target_include_directories(jadidi PRIVATE
        engine
        /usr/include/SDL2
)

target_link_libraries(jadidi PRIVATE
        SDL2
        SDL2main
        SDL2_image
        SDL2_ttf
)