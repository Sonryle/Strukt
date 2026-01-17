cmake_minimum_required(VERSION 3.12)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 17)
project(testbed)

file(GLOB SRC_FILES "src/*.c")
file(GLOB HEADER_FILES "include/*.h")
add_executable(testbed ${SRC_FILES} ${HEADER_FILES})
# target_include_directories(testbed PRIVATE include)

# add Strukt library
add_subdirectory(${CMAKE_SOURCE_DIR}/../Strukt ${CMAKE_BINARY_DIR}/Strukt)

# link library into the project
target_link_libraries(testbed PRIVATE strukt)

# Set our project as the Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
