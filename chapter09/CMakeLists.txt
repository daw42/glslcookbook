set( chapter09_SOURCES 
	main.cpp
	scenefire.cpp
	sceneparticles.cpp
	sceneparticlesfeedback.cpp
	sceneparticlesinstanced.cpp
	scenesmoke.cpp
	scenewave.cpp
)

add_executable( chapter09 ${chapter09_SOURCES} )
target_link_libraries( chapter09 ${GLSLCOOKBOOK_LIBS} )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
