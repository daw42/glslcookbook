set( chapter10_SOURCES 
	main.cpp
	scenemandelbrot.cpp
)

add_executable( chapter10 ${chapter10_SOURCES} )
target_link_libraries( chapter10 ${GLSLCOOKBOOK_LIBS} )
