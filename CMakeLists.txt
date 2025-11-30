cmake_minimum_required(VERSION 3.10)
project(voxelEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0072)
        cmake_policy(SET CMP0072 NEW)
endif()

find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)


file(GLOB_RECURSE SOURCES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/thirdParty/imgui/*.cpp
)

add_executable(voxelEngine ${SOURCES}
        include/PlayerController.hpp
        src/PlayerController.cpp
        include/BlockTypes.hpp
        src/ThreadPool.cpp
        include/GuiManager.hpp
        src/GuiManager.cpp
        include/GpuProfiler.hpp)

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


include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/thirdParty)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)


target_link_libraries(voxelEngine PRIVATE glfw ${OPENGL_LIBRARIES})