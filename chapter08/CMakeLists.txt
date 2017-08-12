set(target chapter08)
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

add_executable( ${target} ${chapter08_SOURCES} )

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
