set( chapter05_SOURCES 	
	scenebloom.cpp
	main.cpp
	sceneblur.cpp
	scenedeferred.cpp
	sceneedge.cpp
	scenegamma.cpp
	scenemsaa.cpp
        scenetonemap.cpp
        scenehdrbloom.cpp)

add_executable( chapter05 ${chapter05_SOURCES} )
target_link_libraries( chapter05 ${GLSLCOOKBOOK_LIBS} )
