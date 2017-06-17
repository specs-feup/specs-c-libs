cmake_minimum_required(VERSION 3.2)

# Declare map macros
macro(add_specs_lib_internal _KEY _VALUE)
  set("specs_libs_${_KEY}" "${_VALUE}")
  list (APPEND LIB_MAP_VALUES "${_KEY}")
endmacro(add_specs_lib_internal)

macro(GET_FROM_MAP _KEY _OUTPUT)
  if(NOT (DEFINED specs_libs_${_KEY}))
	message(FATAL_ERROR "SPeCS-C library '${_KEY}' not defined. Available libs: ${LIB_MAP_VALUES}")
  endif()
  set("${_OUTPUT}" "${specs_libs_${_KEY}}")
endmacro(GET_FROM_MAP)



# Add libraries

add_specs_lib_internal("specs_rapl" "SpecsRapl")
add_specs_lib_internal("specs_logger" "SpecsLogger")

