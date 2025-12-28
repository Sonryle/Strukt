cmake_minimum_required(VERSION 3.12)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
project(strukt)

file(GLOB_RECURSE SRC_FILES "src/*.c")
file(GLOB_RECURSE HEADER_FILES "src/*.h")
add_executable(strukt ${SRC_FILES} ${HEADER_FILES})
target_include_directories(strukt PUBLIC ./src)

add_subdirectory(dependencies)
target_link_libraries(strukt glfw)
target_link_libraries(strukt glad)

# Set our project as the Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
