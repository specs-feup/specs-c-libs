cmake_minimum_required(VERSION 3.0)
project(SPECS_LOGGER_TEST)


set(CMAKE_CXX_STANDARD 11)

# Path to the root of the repository
set(PATH_TO_ROOT "../..")

# Include specs-c
include(${PATH_TO_ROOT}/specs-c-libs.cmake)


set(SOURCE_FILES src/main.cpp)

add_executable(specs_logger_test ${SOURCE_FILES})

add_specs_c_lib(specs_logger_test specs_logger)
