# This Finder allow to find SOFA files for build a project with it.
# In order to use this script, you have to call the find_package(SOFA) command in your CMakeLists.txt
#
# This module defines for use :
# SOFA_LIBRARIES wich contain all LIBRARIES variables in absolute path of Sofa
# SOFA_FOUND, if false, Sofa not found
# SOFA_INCLUDE_DIRS, where to find all the headers
#
# $SOFA_DIR is the entry point of this find package.
# $SOFA_DIR can be set by an environment variable path or in command line
#
# Header files are presumed to be included like
# #include <sofa/defaulttype/Vec.h>
# #include <sofa/defaulttype/Quat.h>
#
# To maintain this script, you just have to :
#  * update if necessary the header file and path to search it to find the framwork/moules/application dirs
#  * update if necessary the include dirs for extlibs
#  * update if necessary name and cmake name of libraries in the corresponding section
#  * update if necessary the paths to search the libs to the find_lib macro
#
# If you have some problem to load the include files, be sur you had append CMAKE_MODULE_PATH to the cmake dir :
# list(APPEND CMAKE_MODULE_PATH "${SOFA_DIR}/cmake")
#
# Created by Jean-Christophe Lombardo and Jerome Esnault.



## ###########################################################################################
## VERBOSITY SETTINGS
## ###########################################################################################
option(VERBOSE_SOFA "Do you want cmake to be verbose during project searching?" false)
message(STATUS "VERBOSE_SOFA = ${VERBOSE_SOFA}")


## ###########################################################################################
## DEFINE SOFA_DIR root path
## ###########################################################################################
if (NOT SOFA_DIR)
    set(SOFA_DIR "$ENV{SOFA_DIR}" CACHE PATH "Sofa root directory")
endif (NOT SOFA_DIR)
## Make sur the provided path is a cmake style path with unix /
file(TO_CMAKE_PATH ${SOFA_DIR} SOFA_DIR)

if(SOFA_DIR AND VERBOSE_SOFA)
    message( STATUS "\nSOFA_DIR = ${SOFA_DIR}" )
    message( STATUS "" )
endif(SOFA_DIR AND VERBOSE_SOFA)

if(NOT SOFA_DIR)
    message( "" )
    message( "Set SOFA_DIR because it : ${SOFA_DIR}" )
    message( "" )
endif(NOT SOFA_DIR)

## Already in cache, be silent
IF (SOFA_INCLUDE_DIRS)
    set(SOFA_FIND_QUIETLY TRUE)
    if (VERBOSE_SOFA)
        message( STATUS "SOFA_INCLUDE_DIRS already in cache : ${SOFA_INCLUDE_DIRS}" )
    endif (VERBOSE_SOFA)
ENDIF (SOFA_INCLUDE_DIRS)


## ###########################################################################################
## FIND INCLUDE SOFA DIRS
## ###########################################################################################
if (VERBOSE_SOFA)
    message(STATUS "----SOFA INCLUDE DIRS LIST : ")
endif (VERBOSE_SOFA)


## ===================== SOFA_INCLUDE_FRAMEWORK_DIR
find_path(SOFA_INCLUDE_FRAMEWORK_DIR 
   NAME
        sofa/core/core.h #use a file .h looks like important file to find the path directory
   PATHS
        ${SOFA_DIR}/framework
        ## comment to allow CMake to search in system environment variables and
        ## in cmake cache/environment/defined variables
		#NO_DEFAULT_PATH
)
if (VERBOSE_SOFA)
    message( STATUS "SOFA_INCLUDE_FRAMEWORK_DIR = ${SOFA_INCLUDE_FRAMEWORK_DIR}" )
endif (VERBOSE_SOFA)


## ===================== SOFA_INCLUDE_MODULES_DIR
find_path(SOFA_INCLUDE_MODULES_DIR 
   NAME
        sofa/component/init.h #use a file .h looks like important file to find the path directory
   PATHS
        ${SOFA_DIR}/modules
        ## comment to allow CMake to search in system environment variables
        ## and in cmake cache/environment/defined variables
		#NO_DEFAULT_PATH
)
if (VERBOSE_SOFA)
    message( STATUS "SOFA_INCLUDE_MODULES_DIR = ${SOFA_INCLUDE_MODULES_DIR}" )
endif (VERBOSE_SOFA)


## ===================== SOFA_INCLUDE_APPLICATIONS_DIR
find_path(SOFA_INCLUDE_APPLICATIONS_DIR
   NAME
        sofa/gui/SofaGUI.h #use a file .h looks like important file to find the path directory
   PATHS
        ${SOFA_DIR}/applications
        ## comment to allow CMake to search in system environment variables and
        ## in cmake cache/environment/defined variables
		#NO_DEFAULT_PATH
)
if (VERBOSE_SOFA)
    message( STATUS "SOFA_INCLUDE_APPLICATIONS_DIR = ${SOFA_INCLUDE_APPLICATIONS_DIR}" )
endif (VERBOSE_SOFA)


## ===================== SOFA_INCLUDE_OTHER_DIRS
set(SOFA_INCLUDE_EXTLIBS "${SOFA_DIR}/extlibs")
list(APPEND SOFA_INCLUDE_OTHER_DIRS
    ${SOFA_INCLUDE_EXTLIBS}
	${SOFA_INCLUDE_EXTLIBS}/ARTrack
    ${SOFA_INCLUDE_EXTLIBS}/CImg
	${SOFA_INCLUDE_EXTLIBS}/colladadom
	${SOFA_INCLUDE_EXTLIBS}/csparse
	${SOFA_INCLUDE_EXTLIBS}/cudpp
    #${SOFA_INCLUDE_EXTLIBS}/eigen-3.0.1
	${SOFA_INCLUDE_EXTLIBS}/eigen-3.0.3
	${SOFA_INCLUDE_EXTLIBS}/ffmpeg
	${SOFA_INCLUDE_EXTLIBS}/fftpack
	${SOFA_INCLUDE_EXTLIBS}/fishpack
	${SOFA_INCLUDE_EXTLIBS}/indexedmap
	${SOFA_INCLUDE_EXTLIBS}/libQGLViewer-2.3.3
	${SOFA_INCLUDE_EXTLIBS}/LML
	${SOFA_INCLUDE_EXTLIBS}/metis
	${SOFA_INCLUDE_EXTLIBS}/miniBoost
	${SOFA_INCLUDE_EXTLIBS}/miniFlowVR/include
	${SOFA_INCLUDE_EXTLIBS}/MKL
	${SOFA_INCLUDE_EXTLIBS}/muparser
	${SOFA_INCLUDE_EXTLIBS}/newmat
	${SOFA_INCLUDE_EXTLIBS}/PML
    #${SOFA_INCLUDE_EXTLIBS}/qwt-5.2.0/src
	${SOFA_INCLUDE_EXTLIBS}/qwt-6.0.1/src
	${SOFA_INCLUDE_EXTLIBS}/self-ccd-1.0
#	${SOFA_INCLUDE_EXTLIBS}/SLC
	${SOFA_INCLUDE_EXTLIBS}/taucs
	${SOFA_INCLUDE_EXTLIBS}/taucs_mt
	${SOFA_INCLUDE_EXTLIBS}/taucs-svn
	${SOFA_INCLUDE_EXTLIBS}/tinyxml
	${SOFA_INCLUDE_EXTLIBS}/VRPN
	${SOFA_INCLUDE_EXTLIBS}/wiiuse
)
if(MSVC)
	list(APPEND SOFA_INCLUDE_OTHER_DIRS ${SOFA_DIR}/include)
endif(MSVC)


list(LENGTH SOFA_INCLUDE_OTHER_DIRS otherIncludeCount)
math(EXPR count "${otherIncludeCount}-1")
foreach( i RANGE 0 ${count})
    list(GET SOFA_INCLUDE_OTHER_DIRS ${i} VALUE)
    if(EXISTS ${VALUE})
        if(VERBOSE_SOFA)
            message(STATUS "SOFA_INCLUDE_OTHER_DIRS_${i} = ${VALUE}")
        endif()
        set(SOFA_INCLUDE_OTHER_DIRS_${i} ${VALUE} CACHE STRING "other include dirs needed by sofa")
    else()
        message(WARNING 
                "It seems the SOFA_INCLUDE_OTHER_DIRS_${i} doest not exist : ${VALUE}.
                Please check it in FindSOFA.cmake")
    endif()
endforeach()
if(VERBOSE_SOFA)
    message( STATUS "----set this variable : SOFA_INCLUDE_DIRS with all include directories found" )
    message(STATUS "")
endif()


set(SOFA_INCLUDE_DIRS
    ${SOFA_INCLUDE_FRAMEWORK_DIR}
    ${SOFA_INCLUDE_MODULES_DIR}
    ${SOFA_INCLUDE_APPLICATIONS_DIR}
    ${SOFA_INCLUDE_OTHER_DIRS}
)

## ###########################################################################################
## FIND LIBRARIES
##
## SOFA group the components by functionality and maturity state.
## 50 new groups are contained in 5 different categories:
## BASE, COMMON, GENERAL, ADVANCED and MISC.
##
## 1- collect all library name to search in the SOFA_LIBS_NAME list splitted into 5 parts
##	* the SOFA LIBS BASE LIST
##	* the SOFA COMMON LIST
##	* THE SOFA GENERAL LIST
##	* THE SOFA ADVANCED LIST
##	* THE SOFA MISC LIST
## 2- for each library :
##	* get it CMAKE_SOFA_LIB_NAME and it associate REAL_SOFA_LIB_NAME
##	* find library and set SOFA_LIBRARIES
## ###########################################################################################
## Put the name of the library SOFA CORE to search and put it associate CMakeName
list(APPEND SOFA_LIB_NAME
	sofabgl 		SOFA_LIB_BGL_LIBRARY
	sofagui 		SOFA_LIB_GUI_LIBRARY
	sofatree 		SOFA_LIB_TREE_LIBRARY
	sofacore 		SOFA_LIB_CORE_LIBRARY
	sofaguiqt 		SOFA_LIB_GUI_QT_LIBRARY
	sofahelper 		SOFA_LIB_HELPER_LIBRARY
	sofaguiglut 	SOFA_LIB_GUI_GLUT_LIBRARY
	sofaguimain 	SOFA_LIB_GUI_MAIN_LIBRARY
	sofamodeler 	SOFA_LIB_MODELER_LIBRARY
	sofasimulation 	SOFA_LIB_SIMULATION_LIBRARY
	sofadefaulttype SOFA_LIB_DEFAULT_TYPE_LIBRARY
)
list(LENGTH SOFA_LIB_NAME sofaLibList)
math(EXPR passToBase "${sofaLibList}/2")


## Put the name of the library SOFA BASE to search and put it associate CMakeName
list(APPEND SOFA_LIB_BASE_NAME
	sofa_base_animation_loop 	SOFA_LIB_BASE_ANIMATION_LOOP_LIBRARY
	sofa_base_collision  	 	SOFA_LIB_BASE_COLLISION_LIBRARY
	sofa_base_linear_solver 	SOFA_LIB_BASE_LINEAR_SOLVER_LIBRARY
	sofa_base_mechanics  		SOFA_LIB_BASE_MECHANICS_LIBRARY
	sofa_base_topology  		SOFA_LIB_BASE_TOPOLOGY_LIBRARY
	sofa_base_visual  			SOFA_LIB_BASE_VISUAL_LIBRARY
	sofa_component  			SOFA_LIB_COMPONENT_LIBRARY

)
list(LENGTH SOFA_LIB_BASE_NAME sofaLibBaseList)
math(EXPR passToCommon "${sofaLibBaseList}/2+${passToBase}")


## Put the name of the library SOFA COMMON to search and put it associate CMakeName
list(APPEND SOFA_LIB_COMMON_NAME
	sofa_deformable				SOFA_LIB_COMMON_DEFORMABLE_LIBRARY
	sofa_explicit_ode_solver	SOFA_LIB_COMMON_ODE_SOLVER_LIBRARY
	sofa_implicit_ode_solver	SOFA_LIB_COMMON_IMPLICIT_ODE_SOLVER_LIBRARY
	sofa_loader					SOFA_LIB_COMMON_LOADER_LIBRARY
	sofa_mesh_collision			SOFA_LIB_COMMON_MESH_COLLISION_LIBRARY
	sofa_rigid					SOFA_LIB_COMMON_RIGID_LIBRARY
	sofa_simple_fem				SOFA_LIB_COMMON_SIMPLE_FEM_LIBRARY
	sofa_object_interaction		SOFA_LIB_COMMON_OBJECT_INTERACTION_LIBRARY
)
list(LENGTH SOFA_LIB_COMMON_NAME sofaCommonLibList)
math(EXPR passToGeneralLib "${sofaCommonLibList}/2+${passToCommon}")


## Put the name of the library SOFA GENERAL to search and put it associate CMakeName
list(APPEND SOFA_LIB_GENERAL_NAME
	sofa_boundary_condition		SOFA_LIB_GENERAL_BOUNDARY_CONDITION_LIBRARY
	sofa_constraint				SOFA_LIB_GENERAL_CONSTRAINT_LIBRARY
	sofa_dense_solver			SOFA_LIB_GENERAL_DENSE_SOLVER_LIBRARY
	sofa_engine					SOFA_LIB_GENERAL_ENGINE_LIBRARY
	sofa_exporter				SOFA_LIB_GENERAL_EXPORTER_LIBRARY
	sofa_graph_component		SOFA_LIB_GENERAL_GRAPH_COMPONENT_LIBRARY
	sofa_opengl_visual			SOFA_LIB_GENERAL_OPENGL_VISUAL_LIBRARY
	sofa_preconditioner			SOFA_LIB_GENERAL_PRECONDITIONER_LIBRARY
#?	sofa_sparse_solver			SOFA_LIB_GENERAL_SPARSE_SOLVER_LIBRARY #optional
	sofa_topology_mapping		SOFA_LIB_GENERAL_TOPOLOGY_MAPPING_LIBRARY
	sofa_user_interaction		SOFA_LIB_GENERAL_USER_INTERACTION_LIBRARY
	sofa_validation				SOFA_LIB_GENERAL_VALIDATION_LIBRARY
	sofa_haptics				SOFA_LIB_GENERAL_HAPTICS_LIBRARY
)
list(LENGTH SOFA_LIB_GENERAL_NAME sofaGeneralLibList)
math(EXPR passToAdvancedLib "${sofaGeneralLibList}/2+${passToGeneralLib}")


## Put the name of the library SOFA ADVANCED to search and put it associate CMakeName
list(APPEND SOFA_LIB_ADVANCED_NAME
	sofa_advanced_constraint	SOFA_LIB_ADVANCED_CONSTRAINT_LIBRARY
	sofa_advanced_fem			SOFA_LIB_ADVANCED_FEM_LIBRARY
	sofa_advanced_interaction	SOFA_LIB_ADVANCED_INTERACTION_LIBRARY
	sofa_eigen2_solver			SOFA_LIB_EIGEN2_SOLVER_LIBRARY
	sofa_eulerian_fluid			SOFA_LIB_EULERIAN_FUILD_LIBRARY
	sofa_mjed_fem				SOFA_LIB_MJED_FEM_LIBRARY
	sofa_non_uniform_fem		SOFA_LIB_NON_UNIFORM_FEM_LIBRARY
#?	sofa_pardiso_solver			SOFA_LIB_PARDISO_SOLVER_LIBRARY #optional
	sofa_sph_fluid				SOFA_LIB_SPH_FUILD_LIBRARY
#?	sofa_taucs_solver			SOFA_LIB_TAUCS_SOLVER_LIBRARY #optional
	sofa_volumetric_data		SOFA_LIB_VOLUMETRIC_DATA_LIBRARY
)
list(LENGTH SOFA_LIB_GENERAL_NAME sofaAdvancedLibList)
math(EXPR passToMiscLib "${sofaAdvancedLibList}/2+${passToAdvancedLib}")


## Put the name of the library SOFA MISC to search and put it associate CMakeName
list(APPEND SOFA_LIB_MISC_NAME
	sofa_misc				SOFA_LIB_MISC_LIBRARY
	sofa_misc_collision		SOFA_LIB_MISC_COLLISION
	sofa_misc_fem			SOFA_LIB_MISC_FEM
	sofa_misc_forcefield	SOFA_LIB_MISC_FORCEFIELD
	sofa_misc_mapping		SOFA_LIB_MISC_MAPPING
	sofa_misc_solver		SOFA_LIB_MISC_SOLVER
	sofa_misc_topology		SOFA_LIB_MISC_TOPOLOGY
	sofa_misc_engine		SOFA_LIB_MISC_ENGINE
)
list(LENGTH SOFA_LIB_MISC_NAME sofaMiscLibList)
math(EXPR passToExtLib "${sofaMiscLibList}/2+${passToMiscLib}")


## Put the name of the library EXT to search and put it associate CMakeName
if(MSVC)
	list(APPEND SOFA_LIB_MSVC
	iconv 	SOFACOMMON_ICONV
	glew32 	SOFACOMMON_GLEW
	glut32 	SOFACOMMON_GLUT
	libxml2 SOFACOMMON_LIBXML2
	)	
else(MSVC)
	set(SOFA_LIB_MSVC)
endif(MSVC)

list(APPEND SOFA_LIB_EXT_NAME
	qwt 		SOFA_LIB_EXT_QWT_LIBRARY
	miniFlowVR 	SOFA_LIB_EXT_MINI_FLOWVR_LIBRARY
	newmat 		SOFA_LIB_EXT_NEWMAT_LIBRARY
	tinyxml 	SOFA_LIB_EXT_TINYXML_LIBRARY
	${SOFA_LIB_MSVC}
)


## Collect all list of libs names together in one list
list(APPEND SOFA_LIBS_NAME 
	${SOFA_LIB_NAME}
	${SOFA_LIB_BASE_NAME}
	${SOFA_LIB_COMMON_NAME}
	${SOFA_LIB_GENERAL_NAME}
	${SOFA_LIB_ADVANCED_NAME}
	${SOFA_LIB_MISC_NAME}
	${SOFA_LIB_EXT_NAME}
)

# To use the find_lib macro from find_lib.cmake
include(find_lib)

list(APPEND SEARCH_LIB_PATHS
    ${SOFA_DIR}/lib
    ${SOFA_DIR}/lib/linux
    ${SOFA_DIR}/lib/linux/sofa-plugins
    ${SOFA_DIR}/lib/win32
    ${SOFA_DIR}/lib/win32/Common
    ${SOFA_DIR}/lib/win32/ReleaseVC7
    ${SOFA_DIR}/lib/win32/ReleaseVC8
    ${SOFA_DIR}/lib/win32/ReleaseVC9
    #${SOFA_DIR}/bin
)


## Take the name of the library to found in the list[n],
## found it full path and place it into the variable of the list[n+1]
list(LENGTH SOFA_LIBS_NAME lengthList)
math(EXPR count "${lengthList}/2-1") #because of the foreach expression to take namesLib by pair
if(VERBOSE_SOFA)
    message(STATUS "")
    message(STATUS "----SOFA LIBS LIST : ")
endif()
foreach(index RANGE 0 ${count} 1)
	
	## Indices to display list in 3 parts
    if(VERBOSE_SOFA)
        if(index EQUAL passToBase)
            message(STATUS "")
            message(STATUS "----SOFA BASE LIBS LIST : ")
        endif()
        if(index EQUAL passToCommon)
            message(STATUS "")
            message(STATUS "----SOFA COMMON LIBS LIST : ")
        endif()
        if(index EQUAL passToGeneralLib)
            message(STATUS "")
            message(STATUS "----SOFA GENERAL LIBS LIST : ")
        endif()
        if(index EQUAL passToAdvancedLib)
            message(STATUS "")
            message(STATUS "----SOFA ADVANCED LIBS LIST : ")
        endif()
        if(index EQUAL passToMiscLib)
            message(STATUS "")
            message(STATUS "----SOFA MISC LIBS LIST : ")
        endif()
        if(index EQUAL passToExtLib)
            message(STATUS "")
            message(STATUS "----SOFA EXT LIBS LIST : ")
        endif()
    endif(VERBOSE_SOFA)

	## Get the valueName of the searched library
	math(EXPR indexValue "${index}*2")
	list(GET SOFA_LIBS_NAME ${indexValue} REAL_SOFA_LIB_NAME)
	
	## Get the variableName of the coresponding searched library
	math(EXPR indexName "${index}*2+1")
	list(GET SOFA_LIBS_NAME ${indexName} CMAKE_SOFA_LIB_NAME)
	
	## Allow to select library
#    option(SOFA_USE_${CMAKE_SOFA_LIB_NAME} "Build ${CMAKE_SOFA_LIB_NAME}" true)
#	if(SOFA_USE_${CMAKE_SOFA_LIB_NAME})
	
		## Use the MACRO to find the library with it full path
        FIND_LIB(${CMAKE_SOFA_LIB_NAME} ${REAL_SOFA_LIB_NAME}
                PATHSLIST_DEBUG
                    ${SEARCH_LIB_PATHS}
                    ${SOFA_DIR}/lib/Debug
                    ${SOFA_DIR}/lib64/Debug
                PATHSLIST_RELEASE
                    ${SEARCH_LIB_PATHS}
                    ${SOFA_DIR}/lib/Release
                    ${SOFA_DIR}/lib64/Release
                VERBOSE         ${VERBOSE_SOFA}
                FORCE_DEBUG     true
                FORCE_RELEASE   true
        )
        
        if(NOT EXISTS ${${CMAKE_SOFA_LIB_NAME}_DEBUG} OR NOT EXISTS ${${CMAKE_SOFA_LIB_NAME}})
            message(WARNING 
                "It seems the ${CMAKE_SOFA_LIB_NAME} does not exist : ${${CMAKE_SOFA_LIB_NAME}}.
                Please, check it in FindSOFA.cmake.")
        endif()
	
		## Add all libraries (release and then debug) find to one variable
        if(${CMAKE_SOFA_LIB_NAME}_DEBUG)
		    list(APPEND SOFA_LIBRARIES
		            optimized ${${CMAKE_SOFA_LIB_NAME}}
		            debug     ${${CMAKE_SOFA_LIB_NAME}_DEBUG}
		    )
        else()
		    list(APPEND SOFA_LIBRARIES ${${CMAKE_SOFA_LIB_NAME}})
        endif()

#	endif(SOFA_USE_${CMAKE_SOFA_LIB_NAME})
	
endforeach(index)

if(VERBOSE_SOFA)
    message(STATUS "----set this variable : SOFA_LIBRARIES with all libraries found")
    message(STATUS "")
endif()


## ###########################################################################################
## FINALISE AND CHECK
## ###########################################################################################
## handle the QUIETLY and REQUIRED arguments and set SOFA_FOUND to TRUE if 
## all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SOFA DEFAULT_MSG
		SOFA_INCLUDE_FRAMEWORK_DIR
        SOFA_INCLUDE_MODULES_DIR
        SOFA_INCLUDE_APPLICATIONS_DIR        
        SOFA_LIB_CORE_LIBRARY
        SOFA_LIB_HELPER_LIBRARY
        SOFA_LIB_SIMULATION_LIBRARY
        SOFA_LIB_COMPONENT_LIBRARY
)


## ###########################################################################################
## FIND SOFA DEFINES (this can be skip by comment the 2 lines)
## ###########################################################################################
include(findSofaDefines)
findSofaDefines(${VERBOSE_SOFA})
