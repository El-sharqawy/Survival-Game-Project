cmake_minimum_required(VERSION 3.28)
project(MyGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Find SDL2 && OpenGL
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)

SET(GCC_COVERAGE_COMPILE_FLAGS "-fpermissive")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# Include SDL2 & OpenGL headers
include_directories(${SDL2_INCLUDE_DIRS})

# Add your source files
add_executable(MyGame main.cpp
        game.h
        Engine.cpp
        Engine.h
        Debug/Config.cpp
        Debug/Config.h
        Debug/Log.cpp
        Debug/Log.h
        Debug/Timer.cpp
        Debug/Timer.h
        BaseLib/Vector3D.cpp
        BaseLib/Vector3D.h
        BaseLib/Vector2D.cpp
        BaseLib/Vector2D.h
        BaseLib/CollisionPlane.cpp
        BaseLib/CollisionPlane.h
        BaseLib/CollisionSphere.cpp
        BaseLib/CollisionSphere.h
        BaseLib/Collision.cpp
        BaseLib/Collision.h
        BaseLib/Camera.cpp
        BaseLib/Camera.h
        BaseLib/ObjectLoader.cpp
        BaseLib/ObjectLoader.h
        BaseLib/Map.cpp
        BaseLib/Map.h
        BaseLib/SkyBox.cpp
        BaseLib/SkyBox.h
        BaseLib/upng.h
        BaseLib/upng.cpp
        UserLib/Player.cpp
        UserLib/Player.h
        Debug/Filename.h
        BaseLib/Utils.h
        Utils/stb_image.cpp
        Utils/linmath.h
        Utils/stb_image_write.cpp
        Utils/stb_image.h
        Utils/stb_image_write.h
        Utils/glad.h
        Utils/glad.c
)

# Link SDL2 libraries
target_link_libraries(MyGame ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} OpenGL::GL OpenGL::GLU)
