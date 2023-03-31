cmake_minimum_required(VERSION 3.20)
project(shaderedit VERSION 0.0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Vulkan
find_package(Vulkan REQUIRED)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" FORCE)
message(STATUS "Installing glfw via submodule")
add_subdirectory(external/glfw)

# fmt
message(STATUS "Installing fmt via submodules")
add_subdirectory(external/fmt)


add_executable(shaderedit src/main.cpp src/window.cpp src/app.cpp src/assets.cpp
    src/resource_manager.cpp src/vulkan_device.cpp src/pipeline.cpp)

include_directories("${Vulkan_INCLUDE_DIR}")
target_link_libraries(shaderedit glfw fmt ${Vulkan_LIBRARIES})

target_compile_options(shaderedit PRIVATE -Wall -Wextra)
