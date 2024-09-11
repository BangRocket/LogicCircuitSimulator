cmake_minimum_required(VERSION 3.10)
project(LogicCircuitSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add raylib
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.2.0
)
FetchContent_MakeAvailable(raylib)

# Add source files
add_executable(${PROJECT_NAME} src/main.cpp)

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)
cmake_minimum_required(VERSION 3.10)
project(LogicCircuitSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add raylib
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.2.0
)
FetchContent_MakeAvailable(raylib)

# Add source files
add_executable(${PROJECT_NAME} src/main.cpp)

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)
