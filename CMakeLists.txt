cmake_minimum_required(VERSION 3.10)
project(LogicCircuitSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

# Add compiler flags for debugging and optimization (GCC, Clang, and MSVC)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
endif()

# Add raylib
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.2.0
)
FetchContent_MakeAvailable(raylib)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/gates
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/include
)

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Platform-specific settings
if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm)
endif()

# Set properties for Debug configuration
set_target_properties(${PROJECT_NAME} PROPERTIES 
    DEBUG_POSTFIX "_debug"
)

# Generate compile_commands.json for VSCode to use
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
