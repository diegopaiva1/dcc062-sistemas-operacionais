# Specify the minimum version for CMake
cmake_minimum_required (VERSION 2.6)

# Project's name and programming language
project (dcc-062-so C)

# The following folder will be included
include_directories ("${PROJECT_SOURCE_DIR}/src")

add_executable (dcc062-so ${PROJECT_SOURCE_DIR}/src/main.c)