# Specify the minimum version for CMake
cmake_minimum_required (VERSION 2.6)

# Project's name and programming language
project (dcc062-tp2 C)

# The following folder will be included
include_directories ("${PROJECT_SOURCE_DIR}/tp2")

add_executable (
    dcc062-tp2
    src/main.c
    src/disk.c
    src/inode.c
    src/util.c
    src/vfs.c
    src/myfs.c
    src/disk.h
    src/inode.h
    src/util.h
    src/vfs.h
    src/myfs.h
)
