# specs-c-libs
C/C++ libraries created by SPeCS research group

Import Libraries
=====================

To import the libraries as source:

- Checkout specs-c-libs repository to a folder

- Copy the files "specs-c-libs.cmake" and "specs-c-libs-config.cmake" to the root of the repository where you want to use the libraries

- Open the copy of "specs-c-libs-config.cmake" and modify the variable SPECS_C_LIBS to point to the location of the specs-c-libs repository. E.g., set(SPECS_C_LIBS "../specs-c-libs")

- In the CMakeLists.txt where you want to use the library, include the copy of "specs-c-libs.cmake" in the root of the repository. E.g., include(../specs-c-libs.cmake)

- Add the library with the macro add_specs_c_lib(<target> <library>). E.g., add_specs_c_lib(project_y specs_logger)

- To check what libraries are available, pass a dummy name as a library to obtain the list during cmake call.


Configure Environment
=====================

- The current preferred IDE is CLion (https://www.jetbrains.com/clion/.) Create an account with the University email to have access to all tools.

Windows
-------

- Install MSYS2 as the compatible gcc toolchain (e.g., MSYS2 http://msys2.github.io/)
 -> Install gcc (pacman -S mingw-w64-x86_64-gcc), gdb (mingw-w64-x86_64-gdb) and cmake (mingw-w64-x86_64-cmake)
