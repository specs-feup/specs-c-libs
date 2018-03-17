# SOURCES
set(MI_SRC
        MemoiInstrumenter.c

        # -------
        # HEADERS
        MemoiInstrumenter.h
        )

set(MI_RUN_SRC
        main.c
        )


# LOAD SUBDIRECTORIES
add_subdirectory(cJSON)


# BUILD EXECUTABLE
add_executable(mi-run ${MI_RUN_SRC})
set_target_properties(mi-run PROPERTIES EXCLUDE_FROM_ALL TRUE)


# LINK LIBRARIES FOR EXECUTABLE
target_link_libraries(mi-run mi)


# BUILD LIBRARY
if (SHARED)
    add_library(mi SHARED ${MI_SRC})
else()
    add_library(mi ${MI_SRC})
endif ()

target_include_directories(mi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# LINK LIBRARIES FOR LIBRARY
target_link_libraries(mi cJSON)
if (GTK2_FOUND)
    target_link_libraries(mi ${GTK2_LIBRARIES})
endif ()

