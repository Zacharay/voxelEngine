cmake_minimum_required(VERSION 3.10)
project(voxelEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Set policy for OpenGL preference
if(POLICY CMP0072)
        cmake_policy(SET CMP0072 NEW)
endif()

# Find packages
find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.c
)

# Add the executable
add_executable(voxelEngine ${SOURCES}
        src/Shader.cpp
        include/Shader.hpp
        include/MeshRenderer.hpp
        include/Camera.hpp)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/thirdParty)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)
# Link libraries
target_link_libraries(voxelEngine PRIVATE glfw ${OPENGL_LIBRARIES})

