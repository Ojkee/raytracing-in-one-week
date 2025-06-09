cmake_minimum_required(VERSION 3.15)
project(RayTracingFunctionalCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -pedantic -Wconversion -Wsign-conversion)
endif()

include_directories(include)

file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.hpp)

add_executable(RayTracingFunctionalCpp
    ${SRC_FILES}
    ${HEADER_FILES}
)
