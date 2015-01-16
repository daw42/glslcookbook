Example code from the [OpenGL Shading Language Cookbook][cookbook]
=========================================================

The example code from the [OpenGL Shading Language Cookbook][cookbook],
by David Wolff.

Requirements
-------------
To use these examples, you'll need the following:

* The [GLM Mathematics Library][GLM] version 0.9.6 or later.  Note that versions
  prior to 0.9.6 will not work properly because of a switch from degrees to
  radians.  GLM 0.9.5 will work, but you'll need to add `#define GLM_FORCE_RADIANS`
  prior to including the glm header files.
* The [GLFW][] library version 3.0 or later.

Compiling the examples
----------------------
All recipes have been tested on Linux and Windows (MinGW).
Your help with other platforms is welcome!  Fork
[this project][ghcookbook] on github.

1.  Install [GLFW][] by following the instructions on their [web site][GLFW].
2.  Install the latest version of [GLM][].
3.  Download the example code from [github][ghcookbook], or clone using git.
4.  Run cmake.  If cmake has difficulties finding the GLFW or GLM installations,
    you can set the variables `GLFW3_ROOT` and/or `GLM_ROOT_DIR` to help
    cmake find them.
5.  Compile by running `make`.

Any problems, create an issue on [github][ghcookbook].

Compiling for Windows with Visual Studio 2012
---------------------------------------------
1.  Install [CMake][] win32 installer
2.  Download [GLFW][] 32-bit Window Libraries and extract to C:\Program Files (x86)\GLFW
3.  Download [GLM][] and extract to C:\Program Files (x86)\GLM
4.  Download the example code from [github][ghcookbook], or clone using git.
5.  Run CMake GUI, point the Source Code directory to the location of the code
    you downloaded in the previous step (i.e. C:/Users/hp/Git/glslcookbook)
6.  Point the Build Binaries directory to the location of the code with CMakeFiles
    appended to it (i.e. C:/Users/hp/Git/glslcookbook/CMakeFiles)
7.  Click 'Configure'
8.  Click yes when prompted to create the Build Directory
9.  Specify Visual Studio 11 (VS 11 == VS 2012, VS10 == VS 2010)
    with 'Use default native compilers', then click 'Finish'
10. Once CMake has finished configuring the Project, click 'Generate'
11. Now open CMakeFiles\GLSLCOOKBOOK.sln
12. Build the Project
13. I've found the easiest way to run the examples inside Visual Studio is to right click
    the chapter that you are interested in and click 'Set as Startup Project', and then
    right click the chapter again and click 'Properties'.  Under Configuration Properties,
    click the Debugging option group.  Type the 'Command Argument' of the specific
    example you are trying to run (i.e. 'ads' for the ambient-diffuse-specular example
    from Chapter 2).  Now you can Run the Local Windows Debugger and everything should
    behave as expected.

If you are having trouble with CMake generating the files, double check where you have
the libraries installed.  If you used a different location to extract GLFW and GLM,
then you will have to edit 'cmake\modules\FindGLFW3.cmake' and/or
'cmake\modules\FindGLM.cmake' making sure that you change all back slashes to
forward slashes in each path

OpenGL Function Loading
-----------------------

The OpenGL header file and a function loader for a 4.4 core profile are
included with this project.  They were generated using
[the OpenGL Loader Generator][GLLoadGen].

The code has been fully tested with OpenGL 4.4 and
compiles under Linux with the latest NVIDIA drivers, and will soon (hopefully)
be tested with MinGW on Windows.

[GLM]: http://glm.g-truc.net
[GLFW]:  http://glfw.org
[ghcookbook]:  http://github.com/daw42/glslcookbook
[cookbook]: http://www.packtpub.com/opengl-4-shading-language-cookbook-second-edition/book
[GLLoadGen]:  https://bitbucket.org/alfonse/glloadgen/wiki/Home
[CMake]: http://www.cmake.org/cmake/resources/software.html
