set(target chapter05)
set( chapter05_SOURCES
	main.cpp
	sceneblur.cpp
	scenedeferred.cpp
	sceneedge.cpp
	scenegamma.cpp
	scenemsaa.cpp
	scenetonemap.cpp
	scenehdrbloom.cpp
		sceneoit.cpp)

add_executable( chapter05 ${chapter05_SOURCES} )

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
