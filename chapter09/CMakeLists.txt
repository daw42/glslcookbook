set(target chapter09)
set( chapter09_SOURCES
	main.cpp
	scenefire.cpp
	sceneparticles.cpp
	sceneparticlesfeedback.cpp
	sceneparticlesinstanced.cpp
	scenesmoke.cpp
	scenewave.cpp
)

add_executable( ${target} ${chapter09_SOURCES} )
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
