cmake_minimum_required(VERSION 3.16)
project(jadidi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(Lua REQUIRED)

file(GLOB_RECURSE ENGINE_SOURCES "engine/*.cpp")

add_executable(jadidi
        main.cpp
        ${ENGINE_SOURCES}
        engine/utils/Config.cpp
        engine/utils/Config.h
        engine/utils/math/vector.h
        engine/scene/GameObject.h
        engine/component/Transform.h
        engine/component/Sprite.h
        engine/lua/LuaObject.h
        engine/lua/LuaApi.cpp
        engine/lua/LuaApi.h
        engine/component/Component.h
        engine/component/Factory.h
        engine/component/Animation.h
        engine/component/Animator.h
        engine/component/Animator.cpp
        engine/scene/GameObject.cpp
        engine/component/Factory.cpp
        engine/component/Rigidbody.cpp
        engine/component/Rigidbody.h
        engine/core/State.cpp
        engine/core/State.h
)

target_include_directories(jadidi PRIVATE
        engine
        /usr/include/SDL2
        ${LUA_INCLUDE_DIR}
)

target_link_libraries(jadidi PRIVATE
        SDL2
        SDL2main
        SDL2_image
        SDL2_ttf
        ${LUA_LIBRARIES}
)

target_compile_options(jadidi PRIVATE
    -march=x86-64
    -mtune=generic
)

#target_compile_options(jadidi PRIVATE
#        -march=native
#)
