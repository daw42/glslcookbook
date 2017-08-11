set(target chapter03)
set( chapter03_SOURCES
	scenemultilight.cpp
	sceneperfragment.cpp
	main.cpp
	scenefog.cpp
	scenedirectional.cpp
	scenetoon.cpp
	scenespot.cpp )

add_executable( ${target} ${chapter03_SOURCES} )

target_compile_definitions(${target}
		PRIVATE
		GLFW_INCLUDE_NONE
		)

target_link_libraries( ${target}
		PRIVATE
		ingredients
		glfw
		${OPENGL_gl_LIBRARY}
		)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
