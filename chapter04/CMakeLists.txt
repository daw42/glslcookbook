set( chapter04_SOURCES  
	scenetexture.cpp
	main.cpp
	scenerendertotex.cpp
	scenemultitex.cpp
	scenealphatest.cpp
	scenereflectcube.cpp
	skybox.cpp
	scenenormalmap.cpp
	sceneprojtex.cpp
	scenerefractcube.cpp
	scenesamplerobj.cpp )

add_executable( chapter04 ${chapter04_SOURCES} )
target_link_libraries( chapter04 ${GLSLCOOKBOOK_LIBS} )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
