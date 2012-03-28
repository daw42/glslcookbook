
-- Set this to the location of the links.lua file within your GLSDK
-- installation.
local glsdk_lua = "c:/glsdk/links.lua"

dofile(glsdk_lua)

solution "glslcookbook"
    configurations { "Debug", "Release" }
    defines {"_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS" }
  
    
project( "ingredients" )
    location "ingredients"
    kind "StaticLib"
    language "c++"
    objdir("ingredients/obj")
    files {"ingredients/*.cpp"}
    files {"ingredients/*.hpp"}
    files {"ingredients/*.h"}
    targetname "ingredients"
				        
    UseLibs { "glload", "freeglut", "glm" }
					        
    configuration "windows"
       defines "WIN32"
       links {"glu32", "opengl32", "winmm", "user32" }
         
    configuration "Debug"
       targetdir "ingredients"
       targetsuffix "D"
       defines "_DEBUG"
       flags "Symbols"
					        
   configuration "Release"
       targetdir "ingredients"
       defines "NDEBUG"
       flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue" };


project( "chapter01" )
    location "chapter01"
    kind "ConsoleApp"
    language "c++"
    objdir("chapter01/obj")
    files {"chapter01/*.cpp"}
    files {"chapter01/*.hpp"}
    files {"chapter01/*.h"}
    targetname "chapter01"
    includedirs "ingredients"
    libdirs "ingredients"
				        
    UseLibs { "glload", "freeglut", "glm" }
					        
    configuration "windows"
       defines "WIN32"
       links {"glu32", "opengl32", "winmm", "user32" }
         
    configuration "Debug"
       targetdir "chapter01"
       targetsuffix "D"
       defines "_DEBUG"
       flags "Symbols"
       links "ingredientsD"
					        
   configuration "Release"
       targetdir "chapter01"
       defines "NDEBUG"
       links "ingredients"
       flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue" };
       
project( "chapter02" )
    location "chapter02"
    kind "ConsoleApp"
    language "c++"
    objdir("chapter02/obj")
    files {"chapter02/*.cpp"}
    files {"chapter02/*.hpp"}
    files {"chapter02/*.h"}
    targetname "chapter02"
    includedirs "ingredients"
    libdirs "ingredients"
				        
    UseLibs { "glload", "freeglut", "glm" }
					        
    configuration "windows"
       defines "WIN32"
       links {"glu32", "opengl32", "winmm", "user32" }
         
    configuration "Debug"
       targetdir "chapter02"
       targetsuffix "D"
       defines "_DEBUG"
       flags "Symbols"
       links "ingredientsD"
					        
   configuration "Release"
       targetdir "chapter02"
       defines "NDEBUG"
       links "ingredients"
       flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue" };
       
project( "chapter03" )
    location "chapter03"
    kind "ConsoleApp"
    language "c++"
    objdir("chapter03/obj")
    files {"chapter03/*.cpp"}
    files {"chapter03/*.hpp"}
    files {"chapter03/*.h"}
    targetname "chapter03"
    includedirs "ingredients"
    libdirs "ingredients"
				        
    UseLibs { "glload", "freeglut", "glm" }
					        
    configuration "windows"
       defines "WIN32"
       links {"glu32", "opengl32", "winmm", "user32" }
         
    configuration "Debug"
       targetdir "chapter03"
       targetsuffix "D"
       defines "_DEBUG"
       flags "Symbols"
       links "ingredientsD"
					        
   configuration "Release"
       targetdir "chapter03"
       defines "NDEBUG"
       links "ingredients"
       flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue" };
       
project( "chapter04" )
    location "chapter04"
    kind "ConsoleApp"
    language "c++"
    objdir("chapter04/obj")
    files {"chapter04/*.cpp"}
    files {"chapter04/*.hpp"}
    files {"chapter04/*.h"}
    targetname "chapter04"
    includedirs "ingredients"
    libdirs "ingredients"
				        
    UseLibs { "glload", "freeglut", "glimage", "glm" }
					        
    configuration "windows"
       defines "WIN32"
       links {"glu32", "opengl32", "winmm", "user32" }
         
    configuration "Debug"
       targetdir "chapter04"
       targetsuffix "D"
       defines "_DEBUG"
       flags "Symbols"
       links "ingredientsD"
					        
   configuration "Release"
       targetdir "chapter04"
       defines "NDEBUG"
       links "ingredients"
       flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue" };
       
project( "chapter05" )
    location "chapter05"
    kind "ConsoleApp"
    language "c++"
    objdir("chapter05/obj")
    files {"chapter05/*.cpp"}
    files {"chapter05/*.hpp"}
    files {"chapter05/*.h"}
    targetname "chapter05"
    includedirs "ingredients"
    libdirs "ingredients"
				        
    UseLibs { "glload", "freeglut", "glimage", "glm" }
					        
    configuration "windows"
       defines "WIN32"
       links {"glu32", "opengl32", "winmm", "user32" }
         
    configuration "Debug"
       targetdir "chapter05"
       targetsuffix "D"
       defines "_DEBUG"
       flags "Symbols"
       links "ingredientsD"
					        
   configuration "Release"
       targetdir "chapter05"
       defines "NDEBUG"
       links "ingredients"
       flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue" };
       
project( "chapter06" )
    location "chapter06"
    kind "ConsoleApp"
    language "c++"
    objdir("chapter06/obj")
    files {"chapter06/*.cpp"}
    files {"chapter06/*.hpp"}
    files {"chapter06/*.h"}
    targetname "chapter06"
    includedirs "ingredients"
    libdirs "ingredients"
				        
    UseLibs { "glload", "freeglut", "glimage", "glm" }
					        
    configuration "windows"
       defines "WIN32"
       links {"glu32", "opengl32", "winmm", "user32" }
         
    configuration "Debug"
       targetdir "chapter06"
       targetsuffix "D"
       defines "_DEBUG"
       flags "Symbols"
       links "ingredientsD"
					        
   configuration "Release"
       targetdir "chapter06"
       defines "NDEBUG"
       links "ingredients"
       flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue" };
       
project( "chapter07" )
    location "chapter07"
    kind "ConsoleApp"
    language "c++"
    objdir("chapter07/obj")
    files {"chapter07/*.cpp"}
    files {"chapter07/*.hpp"}
    files {"chapter07/*.h"}
    targetname "chapter07"
    includedirs "ingredients"
    libdirs "ingredients"
				        
    UseLibs { "glload", "freeglut", "glimage", "glm" }
					        
    configuration "windows"
       defines "WIN32"
       links {"glu32", "opengl32", "winmm", "user32" }
         
    configuration "Debug"
       targetdir "chapter07"
       targetsuffix "D"
       defines "_DEBUG"
       flags "Symbols"
       links "ingredientsD"
					        
   configuration "Release"
       targetdir "chapter07"
       defines "NDEBUG"
       links "ingredients"
       flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue" };
       
project( "chapter08" )
    location "chapter08"
    kind "ConsoleApp"
    language "c++"
    objdir("chapter08/obj")
    files {"chapter08/*.cpp"}
    files {"chapter08/*.hpp"}
    files {"chapter08/*.h"}
    targetname "chapter08"
    includedirs "ingredients"
    libdirs "ingredients"
				        
    UseLibs { "glload", "freeglut", "glimage", "glm" }
					        
    configuration "windows"
       defines "WIN32"
       links {"glu32", "opengl32", "winmm", "user32" }
         
    configuration "Debug"
       targetdir "chapter08"
       targetsuffix "D"
       defines "_DEBUG"
       flags "Symbols"
       links "ingredientsD"
					        
   configuration "Release"
       targetdir "chapter08"
       defines "NDEBUG"
       links "ingredients"
       flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue" };
       
project( "chapter09" )
    location "chapter09"
    kind "ConsoleApp"
    language "c++"
    objdir("chapter09/obj")
    files {"chapter09/*.cpp"}
    files {"chapter09/*.hpp"}
    files {"chapter09/*.h"}
    targetname "chapter09"
    includedirs "ingredients"
    libdirs "ingredients"
				        
    UseLibs { "glload", "freeglut", "glimage", "glm" }
					        
    configuration "windows"
       defines "WIN32"
       links {"glu32", "opengl32", "winmm", "user32" }
         
    configuration "Debug"
       targetdir "chapter09"
       targetsuffix "D"
       defines "_DEBUG"
       flags "Symbols"
       links "ingredientsD"
					        
   configuration "Release"
       targetdir "chapter09"
       defines "NDEBUG"
       links "ingredients"
       flags {"OptimizeSpeed", "NoFramePointer", "ExtraWarnings", "NoEditAndContinue" };
