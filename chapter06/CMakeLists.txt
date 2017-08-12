set(target chapter06)
set( chapter06_SOURCES
	scenebezcurve.cpp
	main.cpp
	scenepointsprite.cpp
	scenequadtess.cpp
	sceneshadewire.cpp
	scenesilhouette.cpp
	scenetessteapot.cpp
	scenetessteapotdepth.cpp )

add_executable( ${target} ${chapter06_SOURCES} )

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
