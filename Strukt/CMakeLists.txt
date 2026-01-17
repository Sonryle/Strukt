cmake_minimum_required(VERSION 3.12)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
project(strukt)

file(GLOB SRC_FILES "src/*.c")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

add_library(strukt STATIC ${SRC_FILES} ${HEADER_FILES})
target_include_directories(strukt PUBLIC include)

add_subdirectory(dependencies)
target_link_libraries(strukt glfw)
target_link_libraries(strukt glad)
