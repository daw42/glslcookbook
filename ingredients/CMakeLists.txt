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
	gl_core_4_4.c)

add_library( ingredients STATIC ${ingredients_SOURCES} )
