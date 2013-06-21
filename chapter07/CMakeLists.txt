set( chapter07_SOURCES 
	frustum.cpp
	main.cpp
	sceneao.cpp
	scenejitter.cpp
	scenepcf.cpp
	sceneshadowmap.cpp )

add_executable( chapter07 ${chapter07_SOURCES} )
target_link_libraries( chapter07 ${GLSLCOOKBOOK_LIBS} )
