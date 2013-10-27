set( chapter02_SOURCES  
	main.cpp
	scenesubroutine.cpp
	scenediffuse.cpp
	scenetwoside.cpp
	sceneflat.cpp
	scenediscard.cpp
	sceneads.cpp )

add_executable( chapter02 ${chapter02_SOURCES} )
target_link_libraries( chapter02 ${GLSLCOOKBOOK_LIBS} )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
