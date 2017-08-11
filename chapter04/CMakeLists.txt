set( target chapter04 )
set( chapter04_SOURCES
	scenetexture.cpp
	main.cpp
	scenerendertotex.cpp
	scenemultitex.cpp
	scenealphatest.cpp
	scenereflectcube.cpp
	skybox.cpp
	scenenormalmap.cpp
	sceneprojtex.cpp
	scenerefractcube.cpp
	scenesamplerobj.cpp )

add_executable( ${target} ${chapter04_SOURCES} )
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
