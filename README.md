Example code from the [OpenGL Shading Language Cookbook][cookbook]
=========================================================

The example code from the [OpenGL Shading Language Cookbook][cookbook], 
by David Wolff.   

Requirements
-------------
To use these examples, you'll need the following:

* The [GLM Mathematics Library][GLM].
* The [GLFW][] library version 3.0 or later.
  
Compiling the examples
----------------------
All recipes have been tested on Linux.  I hope to have
them working on Windows soon.  Your help is welcome!  Fork 
[this project][ghcookbook] on github.

1.  Install [GLFW][] by following the instructions on their [web site][GLFW].
2.  Install the latest version of [GLM][].
3.  Download the example code from [github][ghcookbook], or clone using git.
4.  Run cmake.  If cmake has difficulties finding the GLFW or GLM installations,
    you can set the variables `GLFW3_ROOT` and/or `GLM_ROOT_DIR` to help
    cmake find them.
5.  Compile by running `make`.

Any problems, create an issue on [github][ghcookbook].

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
[cookbook]: http://www.packtpub.com/opengl-4-0-shading-language-cookbook/book
[GLLoadGen]:  https://bitbucket.org/alfonse/glloadgen/wiki/Home
