set( chapter08_SOURCES 
	noisetex.cpp
	main.cpp
	scenesky.cpp
	scenedecay.cpp
	scenenightvision.cpp
	scenepaint.cpp
	scenewood.cpp
	scenenoise.cpp
)

add_executable( chapter08 ${chapter08_SOURCES} )
target_link_libraries( chapter08 ${GLSLCOOKBOOK_LIBS} )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
