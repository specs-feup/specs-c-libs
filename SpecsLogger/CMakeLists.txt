cmake_minimum_required(VERSION 3.0)
project(SPECS_LOGGER)


set(CMAKE_CXX_STANDARD 11)



#set(SOURCE_FILES main.cpp Logger.cpp Logger.h)
set(SOURCE_FILES src/SpecsLogger.cpp)

add_library (specs_logger ${SOURCE_FILES})
target_include_directories (specs_logger PUBLIC include)

#add_executable(specs_logger_test ${SOURCE_FILES} src/main.cpp)
