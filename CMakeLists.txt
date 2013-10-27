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
	find_package( GLM REQUIRED )
	list( APPEND GLSLCOOKBOOK_INCLUDE ${GLM_INCLUDE_DIRS} )

	find_package( glfw 3 REQUIRED )
	list( APPEND GLSLCOOKBOOK_INCLUDE ${GLM_INCLUDE_DIR} )
	list( APPEND GLSLCOOKBOOK_LIBS ${GLFW_LIBRARY} )

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
add_subdirectory( chapter10 )

