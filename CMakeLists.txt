cmake_minimum_required(VERSION 3.20)
project(shaderedit VERSION 0.0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Vulkan
find_package(Vulkan REQUIRED)

set(GLSLANGLIBS /usr/local/lib/ CACHE PATH "glslang library directory")
link_directories(${GLSLANGLIBS})
set(GLSLANGLIBS
    glslang
    HLSL
    OSDependent
    OGLCompiler
    SPIRV
    glslang-default-resource-limits
    SPVRemapper SPIRV-Tools SPIRV-Tools-opt
    MachineIndependent
    GenericCodeGen
    shaderc
    shaderc_util)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" FORCE)
message(STATUS "Installing glfw via submodule")
add_subdirectory(external/glfw)

# fmt
message(STATUS "Installing fmt via submodules")
add_subdirectory(external/fmt)

# shader compilation
find_program(GLSLC glslc HINTS /usr/local/bin)

## find all the shader files under the shaders folder
file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/resources/*.frag"
    "${PROJECT_SOURCE_DIR}/resources/*.vert"
    "${PROJECT_SOURCE_DIR}/resources/*.comp"
)

## iterate each shader
foreach(GLSL ${GLSL_SOURCE_FILES})
    message(STATUS "BUILDING SHADER")
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/resources/${FILE_NAME}.spv")
    message(STATUS ${GLSL})
    ##execute glslang command to compile that specific shader
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${GLSLC} ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)

add_executable(shaderedit src/main.cpp src/window.cpp src/app.cpp src/assets.cpp
    src/resource_manager.cpp src/vulkan_device.cpp src/pipeline.cpp src/swap_chain.cpp ${SPIRV_BINARY_FILES})

include_directories("${Vulkan_INCLUDE_DIR}")
target_link_libraries(shaderedit glfw fmt ${Vulkan_LIBRARIES} ${GLSLANGLIBS})

target_compile_options(shaderedit PRIVATE -Wall -Wextra)


