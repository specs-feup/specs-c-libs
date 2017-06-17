#
# Bootstraps specs-c-libs
#
# After including file, the following is available:
# 
# add_specs_c_lib(<lib_name>)
#  - Includes the given specs-c lib. 
#
cmake_minimum_required(VERSION 3.2)

#Initialize specs-c-libs
if(NOT SPECS_C_LIBS_ENABLED)
	include("${CMAKE_CURRENT_LIST_DIR}/specs-c-libs-config.cmake")
	include("${SPECS_C_LIBS}/init.cmake")
endif()
