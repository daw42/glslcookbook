cmake_minimum_required (VERSION 2.8)

project (GLSLCOOKBOOK)

if( GLFW_SEARCH_PATH )
	set( GLFW_LIB_SEARCH_PATH ${GLFW_SEARCH_PATH}/lib )
	set( GLFW_INCLUDE_SEARCH_PATH ${GLFW_SEARCH_PATH}/include )
endif( GLFW_SEARCH_PATH )

set( GLM_INCLUDE_SEARCH_PATH /usr/include /usr/local/include )
if( GLM_SEARCH_PATH )
	list( APPEND GLM_INCLUDE_SEARCH_PATH ${GLM_SEARCH_PATH} )
endif( GLM_SEARCH_PATH )

set( GLSLCOOKBOOK_LIBS ingredients )
set( GLSLCOOKBOOK_INCLUDE ${GLSLCOOKBOOK_SOURCE_DIR}/ingredients )

if( UNIX )
	find_path( GLFW_INCLUDE GLFW/glfw3.h PATHS ${GLFW_INCLUDE_SEARCH_PATH} )
	list( APPEND GLSLCOOKBOOK_INCLUDE ${GLFW_INCLUDE} )
	
	find_path( GLM_INCLUDE glm/glm.hpp PATHS ${GLM_INCLUDE_SEARCH_PATH} )
	list( APPEND GLSLCOOKBOOK_INCLUDE ${GLM_INCLUDE} )
	
	find_library( GLFW_LIB glfw3 PATHS ${GLFW_LIB_SEARCH_PATH} )
	list( APPEND GLSLCOOKBOOK_LIBS ${GLFW_LIB} )

	find_package( OpenGL REQUIRED )
	list( APPEND GLSLCOOKBOOK_LIBS ${OPENGL_gl_LIBRARY} )
	
	find_package( Threads REQUIRED )
	list( APPEND GLSLCOOKBOOK_LIBS ${CMAKE_THREAD_LIBS_INIT} )
	
	find_package( X11 REQUIRED )
	list( APPEND GLSLCOOKBOOK_LIBS ${X11_Xrandr_LIB} ${X11_Xxf86vm_LIB} ${X11_Xi_LIB} )
	
	find_library( RT_LIB rt )
	list( APPEND GLSLCOOKBOOK_LIBS ${RT_LIB} )
		
endif()

include_directories( ${GLSLCOOKBOOK_INCLUDE} )

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

