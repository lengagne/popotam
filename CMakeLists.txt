cmake_minimum_required( VERSION 2.8 )
project (POPOTAM)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++17 -lstdc++ -msse2 -fPIC")

if(CRRI)
	message("We are on the mesocentre")
# 	set(EIGEN3_INCLUDE_DIR "/opt/apps/gcc-4.8.4/eigen-3.3.1/include/eigen3")
     set(EIGEN3_INCLUDE_DIR "/opt/apps/gcc-8.1.0/eigen-3.3.4/include/eigen3")
else(CRRI)
	message("We are on normal computer")
	find_package(Eigen3 REQUIRED)
endif(CRRI)

if (NOT LAZYCPP_FOUND)
    find_package(LazyCPP REQUIRED)
endif()

# 
### QT used for graphical interfaces strings and xml
find_package(Qt4 REQUIRED COMPONENTS QTCORE QTXML QtXmlPatterns QUIET COMPONENTS QTGUI)
#find_package(Qt4 QUIET COMPONENTS QTGUI)

set(QT_DEFINITIONS -DQT_NO_KEYWORDS)
if(QT_QTGUI_FOUND)
set(QT_DEFINITIONS ${QT_DEFINITIONS} -DQT_GUI_FOUND)
endif(QT_QTGUI_FOUND)
add_definitions(${QT_DEFINITIONS})

include(${QT_USE_FILE})



 ## TCMALLOC used to optimized memory allocation
 find_package(PkgConfig REQUIRED)
 pkg_check_modules(TCMALLOC libtcmalloc_minimal)
 if(TCMALLOC_FOUND)
   message("TCMALLOC FOUND")
   include_directories(${TCMALLOC_INCLUDE_DIRS})
   link_directories(${TCMALLOC_LIBRARY_DIRS})
   list(APPEND MG_LIBRARIES ${TCMALLOC_LIBRARIES})
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")
 endif(TCMALLOC_FOUND)

foreach(p LIB INCLUDE)
	set(var CMAKE_INSTALL_${p}DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
	endif()
endforeach()

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")

# include(${QT_USE_FILE})

file( GLOB_RECURSE POPOTAM_SOURCES src/*.cpp )
FILE ( GLOB_RECURSE POPOTAM_HEADERS include/*.h   include/*.hxx )

file( GLOB_RECURSE POPOTAM_BasisFunctions_SOURCES basis_functions/src/*.cpp )
FILE ( GLOB_RECURSE POPOTAM_BasisFunctions_HEADERS basis_functions/include/*.h)

file( GLOB_RECURSE POPOTAM_Solvers_SOURCES solvers/src/*.cpp )
FILE ( GLOB_RECURSE POPOTAM_Solvers_HEADERS solvers/include/*.h)


include_directories(	include
                        basis_functions/include
                        solvers/include
                        ${EIGEN3_INCLUDE_DIR}
                        ${TCMALLOC_INCLUDE_DIRS}
                        ${LazyCPP_INCLUDE_DIRS}
                        )

link_directories(	${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}
                    ${TCMALLOC_LIBRARY_DIRS})


add_library(POPOTAM SHARED ${POPOTAM_SOURCES} ${POPOTAM_HEADERS})
add_library(POPOTAM_BasisFunctions SHARED ${POPOTAM_BasisFunctions_SOURCES} ${POPOTAM_HEADERS})
add_library(POPOTAM_Solvers SHARED ${POPOTAM_Solvers_SOURCES} ${POPOTAM_HEADERS})

target_link_libraries(	POPOTAM_BasisFunctions
                        ${TCMALLOC_LIBRARIES}
                        ${QT_LIBRARIES}
                        )

target_link_libraries(	POPOTAM_Solvers
                        POPOTAM_BasisFunctions
                        ${TCMALLOC_LIBRARIES}
                        ${LazyCPP_LIBRARIES}
                        )                        


target_link_libraries(	POPOTAM
                        POPOTAM_BasisFunctions
                        POPOTAM_Solvers
                        ${TCMALLOC_LIBRARIES}
                        )

                        # ### Configure file
# get_property(Include_Directories DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
SET(POPOTAM_INCLUDE_DIRS ${INSTALL_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} ${TCMALLOC_INCLUDE_DIRS} ${LazyCPP_INCLUDE_DIRS} CACHE INTERNAL "")
SET(POPOTAM_LIBRARY_DIRS  ${CMAKE_INSTALL_LIBDIR}/lib  CACHE INTERNAL "")
SET(POPOTAM_LIBRARIES POPOTAM CACHE INTERNAL "")
SET(POPOTAM_FLAGS ${CMAKE_CXX_FLAGS} CACHE INTERNAL "")
SET(POPOTAM_FOUND "TRUE" CACHE INTERNAL "")

add_subdirectory(apps)
