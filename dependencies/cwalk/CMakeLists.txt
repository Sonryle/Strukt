cmake_minimum_required(VERSION 3.10)
project(cwalk)

# Add the glad library
add_library(cwalk STATIC src/cwalk.c)

# Specify include directories for glad
target_include_directories(cwalk PUBLIC include)
