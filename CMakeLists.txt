cmake_minimum_required(VERSION 3.7)

project(glTF-experiments
        VERSION 0.0.0
        DESCRIPTION "Template of how to create a program using VulkanSceneGraph and CMake"
        LANGUAGES CXX
)

# build all examples into the bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Change the default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# find the vsg
if (VULKAN_SDK)
    set(ENV{VULKAN_SDK} ${VULKAN_SDK})
endif()

find_package(vsg 1.0.0 REQUIRED)

# find the optional vsgXchange that can be used for reading a range of image and 3d model formats and shader compilation
find_package(vsgXchange 1.0.0 QUIET)

# set the use of C++17 globally as all examples require it
set(CMAKE_CXX_STANDARD 17)

# add clobber build target to clear all the non git registered files/directories
add_custom_target(clobber
    COMMAND git clean -d -f -x
)

set(SOURCES
    src/json.cpp
    src/gltf.cpp
    src/main.cpp
)

add_executable(gltf-experiments ${SOURCES})

target_link_libraries(gltf-experiments vsg::vsg)

if (vsgXchange_FOUND)
    target_compile_definitions(gltf-experiments PRIVATE vsgXchange_FOUND)
    target_link_libraries(gltf-experiments vsgXchange::vsgXchange)
endif()

install(TARGETS gltf-experiments
        RUNTIME DESTINATION bin
)

