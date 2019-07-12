# Specify the minimum version for CMake
cmake_minimum_required (VERSION 2.6)

# Project's name and programming language
project (dcc062-tp2 C)

# The following folder will be included
include_directories ("${PROJECT_SOURCE_DIR}/tp-2")

add_executable (
    dcc062-tp2
    src/main.c
    src/lottery.c
    src/process.c
    src/sched.c
    src/lottery.h
    src/process.h
    src/sched.h
)
