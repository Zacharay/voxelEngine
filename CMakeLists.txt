cmake_minimum_required(VERSION 3.10)
project(voxelEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set policy for OpenGL preference
if(POLICY CMP0072)
        cmake_policy(SET CMP0072 NEW)
endif()

# Find packages
find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)

# --- FIX: Corrected 'thirdPart' to 'thirdParty' ---
file(GLOB_RECURSE SOURCES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/thirdParty/imgui/*.cpp
)

# --- CLEANUP: Removed manually listed files that are already in SOURCES ---
add_executable(voxelEngine ${SOURCES}
        include/PlayerController.hpp
        src/PlayerController.cpp)

# Custom commands for copying assets
add_custom_command(
        TARGET voxelEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures
        $<TARGET_FILE_DIR:voxelEngine>/textures
        COMMENT "Copying textures..."
)

add_custom_command(
        TARGET voxelEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:voxelEngine>/shaders
        COMMENT "Copying shaders..."
)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/thirdParty) # Ensure this matches your folder name on disk
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(voxelEngine PRIVATE glfw ${OPENGL_LIBRARIES})