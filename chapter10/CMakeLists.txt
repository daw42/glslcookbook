set( chapter10_SOURCES 
	main.cpp
	scenemandelbrot.cpp
	scenecloth.cpp
	sceneparticles.cpp
        sceneedge.cpp
)

add_executable( chapter10 ${chapter10_SOURCES} )
target_link_libraries( chapter10 ${GLSLCOOKBOOK_LIBS} )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
