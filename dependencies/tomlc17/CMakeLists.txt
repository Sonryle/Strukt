cmake_minimum_required(VERSION 3.10)
project(tomlc17)

# Add the glad library
add_library(tomlc17 STATIC src/tomlc17.c)

# Specify include directories for glad
target_include_directories(glad PUBLIC src)
