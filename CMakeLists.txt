cmake_minimum_required(VERSION 3.16)
project(jadidi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_COMPILE_PCH_IN_DEBUG ON)
set(CMAKE_COMPILE_PCH_IN_RELEASE ON)

set(JADIDI_BUILD_PROFILE "generic" CACHE STRING "Build profile: generic or native")

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_check_modules(LUA REQUIRED lua5.4)

find_package(OpenGL REQUIRED)

list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.jadidi")
set(sol2_DIR "$ENV{HOME}/.jadidi/lib/cmake/sol2")
find_package(sol2 CONFIG REQUIRED)

file(GLOB_RECURSE ENGINE_SOURCES "engine/*.cpp")

add_executable(jadidi
    main.cpp
    ${ENGINE_SOURCES}
    vendor/glad/src/glad.c
)

target_include_directories(jadidi PRIVATE
    engine
    vendor/glad/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
)

target_link_libraries(jadidi PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    OpenGL::GL
    ${LUA_LIBRARIES}
    sol2
)

target_link_directories(jadidi PRIVATE
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
    ${SDL2_TTF_LIBRARY_DIRS}
    ${SDL2_MIXER_LIBRARY_DIRS}
    ${LUA_LIBRARY_DIRS}
)

target_compile_options(jadidi PRIVATE
    ${SDL2_CFLAGS_OTHER}
    ${SDL2_IMAGE_CFLAGS_OTHER}
    ${SDL2_TTF_CFLAGS_OTHER}
    ${SDL2_MIXER_CFLAGS_OTHER}
)

if(JADIDI_BUILD_PROFILE STREQUAL "native")
    target_compile_options(jadidi PRIVATE -march=native -mtune=native)
else()
    target_compile_options(jadidi PRIVATE -march=x86-64 -mtune=generic)
endif()