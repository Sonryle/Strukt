cmake_minimum_required(VERSION 3.12)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
project(newcombe)

# Add the executable
file(GLOB SRC_FILES "src/*.c")
file(GLOB HEADER_FILES "include/*.h")
add_executable(Newcombe ${SRC_FILES} ${HEADER_FILES})

# Set include directories for Newcombe
target_include_directories(Newcombe PRIVATE include)

# Add all libraries
# add_subdirectory(dependencies)

# Set our project as the Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
