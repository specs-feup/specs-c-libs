cmake_minimum_required(VERSION 2.8)
project (SPECS_RAPL)

add_library (specs_rapl src/rapl.c src/rapl_fs.c)
	
target_include_directories (specs_rapl PUBLIC include)

add_executable(specs_rapl_example example/example.c)

target_link_libraries(specs_rapl_example specs_rapl pthread)