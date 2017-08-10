set( ingredients_SOURCES vboteapotpatch.cpp
	vboteapot.cpp
	drawable.cpp 
	vbomeshadj.cpp 
	vbotorus.cpp 
	glutils.cpp 
	vbomesh.cpp 
	glslprogram.cpp 
	vbocube.cpp 
	vboplane.cpp 
        vbosphere.cpp
	bmpreader.cpp
	tgaio.cpp
		cookbookogl.h
		scene.h
		teapotdata.h
		glad/src/glad.c)

add_library( ingredients STATIC ${ingredients_SOURCES} )

target_include_directories( ingredients PUBLIC glad/include )

target_link_libraries( ingredients PUBLIC glm )

