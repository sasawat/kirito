cmake_minimum_required(VERSION 3.10)
project(kirito)

# Add path to search for modules
# set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)

# Find headers
include_directories(SYSTEM ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Werror -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wno-unused-parameter")

enable_testing()
add_subdirectory(tests)
