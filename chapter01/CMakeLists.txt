set( chapter01_SOURCES  
	scenebasic.cpp
	main.cpp
	scenebasic_uniformblock.cpp
	scenebasic_attrib.cpp
	scenebasic_uniform.cpp )

add_executable( chapter01 ${chapter01_SOURCES} )
target_link_libraries( chapter01 ${GLSLCOOKBOOK_LIBS} )
