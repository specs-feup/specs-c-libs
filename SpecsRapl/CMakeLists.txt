cmake_minimum_required(VERSION 2.8)
project (SPECS_RAPL)

add_library (specs_rapl src/rapl.c)
	
target_include_directories (specs_rapl PUBLIC include)