cmake_minimum_required(VERSION 2.8)

# retrieve and set project name
set(projectName)
get_directory_property(properties VARIABLES)
foreach(property ${properties})
	if(NOT property STREQUAL "")
		string(REGEX MATCH "^GLOBAL_PROJECT_PATH_.+$" pathMatch ${property})
		if(NOT pathMatch STREQUAL "")
			if(${pathMatch} STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
				string(SUBSTRING "${pathMatch}" 20 -1 projectName)
				break()
			endif()
		endif()
	endif()
endforeach()
if(projectName STREQUAL "")
	message(FATAL_ERROR "Error while trying to add a non-registered project : ${CMAKE_CURRENT_SOURCE_DIR}")
endif()
project(${projectName})

# useful variables
set(COMPILER_DEFINES "")
set(COMPILER_FLAGS "")
set(LINKER_DEPENDENCIES "")
set(LINKER_FLAGS "")

set(GROUP_BASE_DIR "")

## internal
set(ADDITIONAL_COMPILER_DEFINES "")
set(ADDITIONAL_LINKER_DEPENDENCIES "")

# target location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${SOFA_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${SOFA_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${SOFA_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${SOFA_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${SOFA_BIN_DIR}")
if(UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${SOFA_LIB_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${SOFA_LIB_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${SOFA_LIB_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${SOFA_LIB_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${SOFA_LIB_DIR}")
else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${SOFA_BIN_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${SOFA_BIN_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${SOFA_BIN_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${SOFA_BIN_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${SOFA_BIN_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${SOFA_LIB_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${SOFA_LIB_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${SOFA_LIB_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${SOFA_LIB_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${SOFA_LIB_DIR}")
endif()

# out of source build support
set(CMAKE_INCLUDE_CURRENT_DIR 1)