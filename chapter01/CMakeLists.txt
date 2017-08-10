set(target chapter01)

set( chapter01_SOURCES
	scenebasic.cpp
	main.cpp
	scenebasic_uniformblock.cpp
	scenebasic_attrib.cpp
	scenebasic_uniform.cpp
	scenebasic_debug.cpp )

add_executable( ${target} ${chapter01_SOURCES} )

target_compile_definitions(${target}
		PRIVATE
		${DEFAULT_COMPILE_DEFINITIONS}
		GLFW_INCLUDE_NONE
		)

target_link_libraries( ${target}
        PRIVATE
        ingredients
		glfw
        ${OPENGL_gl_LIBRARY}
		)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
