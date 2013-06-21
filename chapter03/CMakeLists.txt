set( chapter03_SOURCES  
	scenemultilight.cpp
	sceneperfragment.cpp
	main.cpp
	scenefog.cpp
	scenedirectional.cpp
	scenetoon.cpp
	scenespot.cpp )

add_executable( chapter03 ${chapter03_SOURCES} )
target_link_libraries( chapter03 ${GLSLCOOKBOOK_LIBS} )
