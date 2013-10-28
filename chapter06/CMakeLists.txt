set( chapter06_SOURCES
	scenebezcurve.cpp
	main.cpp
	scenepointsprite.cpp
	scenequadtess.cpp
	sceneshadewire.cpp
	scenesilhouette.cpp
	scenetessteapot.cpp
	scenetessteapotdepth.cpp )

add_executable( chapter06 ${chapter06_SOURCES} )
target_link_libraries( chapter06 ${GLSLCOOKBOOK_LIBS} )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
