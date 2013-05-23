Example code from the [OpenGL 4.0 Shading Language Cookbook][cookbook]
=========================================================

This is the example programs from the [OpenGL 4.0 Shading Language
Cookbook][cookbook], by David Wolff.  The source code has been updated to 
use GLFW, and no longer requires Qt.

Requirements
-------------
To use these examples, you'll need the following:

* The [GLM Mathematics Library][GLM].
* A recent version of the [GLFW][] library.
  
Compiling the examples
----------------------
All recipes have been tested on Linux.  I hope to have
them working on Windows (using MinGW) soon.  Your help is welcome!  Fork 
[this project][ghcookbook] on github.

1.  Install [GLFW][] by following the instructions on their [web site][GLFW].
2.  Install the latest version of [GLM][].
3.  Download the example code from [github][ghcookbook], or clone using git.
4.  Edit the `Makefile` to point to your GLFW and GLM installations.
5.  Compile by running `make`.

Any problems, create an issue on [github][ghcookbook].

Changes from the book
------------------------
I've dropped Qt and moved to [GLFW][] in order to make the code more easily
portable.  I've also moved over to gnu make.

Since the time of the book's writing, I have experienced problems
with GLEW under a 4.x core profile.  When doing some research it appears that
there are some fundemental issues with GLEW and core profiles.  Therefore,
I've dropped the dependence on GLEW, and I'm using a pre-packaged header and
loader that was created using [the OpenGL Loader Generator][GLLoadGen].

The code has been fully tested with OpenGL 4.3 and 
compiles under Linux with the latest NVIDIA drivers, and will soon (hopefully) 
be tested with MinGW on Windows. 

The major changes are listed below:

* Dropped dependence on Qt.
* Uses [GLFW][], [GLM][], and [GLLoadGen][](optional).
* Dropped dependence on libnoise for Chapter 8 examples.  Instead, uses the noise
  functions available in [GLM][] > 0.9.3.

[GLM]: http://glm.g-truc.net
[GLFW]:  http://glfw.org
[ghcookbook]:  http://github.com/daw42/glslcookbook
[cookbook]: http://www.packtpub.com/opengl-4-0-shading-language-cookbook/book
[GLLoadGen]:  https://bitbucket.org/alfonse/glloadgen/wiki/Home
