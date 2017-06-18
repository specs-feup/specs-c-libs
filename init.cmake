cmake_minimum_required(VERSION 3.2)

include("${CMAKE_CURRENT_LIST_DIR}/libs.cmake")

# Set flag, to signal that deps is enabled
set(SPECS_C_LIBS_ENABLED 1)

# Declare function require_specs-c
function(require_specs_c)

	if(NOT DEFINED SPECS_C_LIBS)
		message(FATAL_ERROR "[specs-c-libs] Requires variable 'SPECS_C_LIBS' to be defined with the path to the specs-c-libs repository")
	endif()

endfunction()

# Check is SPECS_C_LIBS variable is available
require_specs_c()

# Inform location of specs-c-libs repository
message(STATUS "[specs-c-libs] Repository root folder: ${SPECS_C_LIBS}")


# Declare function add_specs_c_lib
function(add_specs_c_lib library)

	get_from_map(${library} lib_dir)
	
	# Add library as source project
	set(CURRENT_SPECS_LIB_DIR ${SPECS_C_LIBS}/${lib_dir})
	add_subdirectory(${CURRENT_SPECS_LIB_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${library})
	
endfunction()

# Declare macro for adding libraries
# Using macro instead of function to be able to use 'target_link_libraries'
macro(add_specs_c_lib target library)

	get_from_map(${library} lib_dir)
	
	# Add library as source project
	set(CURRENT_SPECS_LIB_DIR ${SPECS_C_LIBS}/${lib_dir})
	add_subdirectory(${CURRENT_SPECS_LIB_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${library})
	
	target_link_libraries (${target} ${library})
	
	message(STATUS "[specs-c-libs] Added library ${library} to target ${target}")
endmacro()

