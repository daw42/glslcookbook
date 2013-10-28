cmake_minimum_required (VERSION 2.8)

project (GLSLCOOKBOOK)

set( GLSLCOOKBOOK_LIBS ingredients )
set( GLSLCOOKBOOK_INCLUDE ${GLSLCOOKBOOK_SOURCE_DIR}/ingredients )

# GLM ---------------
find_package(GLM 0.9)
set(GLM_INCLUDE_DIR "${GLM_INCLUDE_DIR}" CACHE PATH "GLM include directory")
if (NOT ${GLM_INCLUDE_DIR})
	find_path(GLM_INCLUDE_DIR glm/glm.hpp)
endif()

list(APPEND GLSLCOOKBOOK_INCLUDE ${GLM_INCLUDE_DIR})


# GLFW --------------
find_package(glfw 3)
set(GLFW_INCLUDE_DIR "${GLFW_INCLUDE_DIR}" CACHE PATH "GLFW include directory")
if (NOT ${GLFW_INCLUDE_DIR})
	find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h)
endif()

set(GLFW_LIBRARY "${GLFW_LIBRARY}" CACHE FILEPATH "GLFW library")
# search for libglfw3 first
if (NOT ${GLFW_LIBRARY})
	find_library(GLFW_LIBRARY glfw3)
endif()
# then search for libglfw3 first
if (NOT ${GLFW_LIBRARY})
	find_library(GLFW_LIBRARY glfw)
endif()

list(APPEND GLSLCOOKBOOK_INCLUDE ${GLFW_INCLUDE_DIR})
list(APPEND GLSLCOOKBOOK_LIBS ${GLFW_LIBRARY})



if(UNIX)
	find_package(OpenGL REQUIRED)
	list(APPEND GLSLCOOKBOOK_LIBS ${OPENGL_gl_LIBRARY})
	
	find_package(Threads REQUIRED)
	list(APPEND GLSLCOOKBOOK_LIBS ${CMAKE_THREAD_LIBS_INIT})
	
	find_package(X11 REQUIRED)
	list(APPEND GLSLCOOKBOOK_LIBS ${X11_Xrandr_LIB} ${X11_Xxf86vm_LIB} ${X11_Xi_LIB})
	
	find_library(RT_LIB rt)
	list(APPEND GLSLCOOKBOOK_LIBS ${RT_LIB})
endif()

include_directories(${GLSLCOOKBOOK_INCLUDE})

add_subdirectory( ingredients )
add_subdirectory( chapter01 )
add_subdirectory( chapter02 )
add_subdirectory( chapter03 )
add_subdirectory( chapter04 )
add_subdirectory( chapter05 )
add_subdirectory( chapter06 )
add_subdirectory( chapter07 )
add_subdirectory( chapter08 )
add_subdirectory( chapter09 )
add_subdirectory( chapter10 )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
