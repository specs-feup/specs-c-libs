# TODO: set build types and corresponding flags
# TODO: make install options
cmake_minimum_required(VERSION 3.2)

message("------------------------------")
message("Building Makefile for MemoiInstrumenter")
message("------------------------------")
message("Source Dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message("Binary Dir: ${CMAKE_CURRENT_BINARY_DIR}")


# PROJECT
project(MemoiInstrumenter C)


# PACKAGES
find_package(GTK2 2.6 REQUIRED gtk)
if (GTK2_FOUND)
    include_directories(${GTK2_INCLUDE_DIRS})
endif ()


# OPTIONS
option(SHARED "Build shared library" OFF)


# OUTPUT FILES
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Output directory for runtime binaries")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Output directory for shared libraries")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Output directory for static libraries")


# BUILD TYPE
set(CMAKE_BUILD_TYPE_INIT "Release")


# COMPILER OPTIONS
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2")
if (SHARED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif ()


# LOAD SUBDIRECTORIES
add_subdirectory(src)
