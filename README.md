Example code from the [OpenGL Shading Language Cookbook][cookbook]
=========================================================

The example code from the [OpenGL Shading Language Cookbook][cookbook],
by David Wolff and published by Packt Publishing, 2013 (ISBN: 9781782167020).

**NEW**:
 * Most examples now run on MacOS!
 * CMake build now uses package config files when finding GLM and GLFW.  This should make for a more
 streamlined configuration.  Use [`CMAKE_PREFIX_PATH`][cmake_prefix] to point CMake to your installs.
 * Examples now properly support high-density displays.

Requirements
-------------
To compile these examples, you'll need the following:

* The [GLM Mathematics Library][GLM] version 0.9.6 or later.  Note that versions
  prior to 0.9.6 may not work properly because of a switch from degrees to
  radians.  GLM 0.9.5 will work, but you'll need to add `#define GLM_FORCE_RADIANS`
  prior to including the glm header files.
* [GLFW][] version 3.0 or later.

Compiling the examples
----------------------
The example code builds with [CMake][].  Note that the
examples for Chapter 10 will not function on MacOS due to lack of support for
compute shaders on that platform.

1.  Install [GLFW][] by following the instructions on their [web site][GLFW].
2.  Install the latest version of [GLM][].  Note that for [CMake][] to find GLM
    correctly, you need to run the install "build" (e.g. `make install`) or install GLM from your
    favorite package manager.  Otherwise, the CMake config files will not be created/available.
3.  Download this example code from [github][ghcookbook], or clone using git.
4.  Run cmake.  If cmake has difficulties finding the GLFW or GLM installations,
    set the variable [`CMAKE_PREFIX_PATH`][cmake_prefix] to help cmake find them.
5.  Compile by running `make`.

Any problems, [create an issue](https://github.com/daw42/glslcookbook/issues) on [github][ghcookbook].

Tips for compiling for Windows with Visual Studio
---------------------------------------------
* Use the Visual Studio target in [CMake][]:  `-G "Visual Studio..."`, open the
  Visual Studio solution.  You should see one project per chapter.
* Each chapter requires a command line argument to choose a recipe.  When
  running in VS, be sure to set the 'Command Argument' under 'Properties' for
  the appropriate recipe.

OpenGL Function Loading
-----------------------

An OpenGL header file and a function loader for a 4.3 core profile are
included with this project.  They were generated using
[GLAD][].  This loader should also work on MacOS under a 4.1 core profile, but of course not all functions will load.

The code has been tested with OpenGL 4.3 on Windows/Linux and OpenGL 4.1 on MacOS.

[GLM]: http://glm.g-truc.net
[GLFW]:  http://glfw.org
[ghcookbook]:  http://github.com/daw42/glslcookbook
[cookbook]: http://www.packtpub.com/opengl-4-shading-language-cookbook-second-edition/book
[GLLoadGen]:  https://bitbucket.org/alfonse/glloadgen/wiki/Home
[CMake]: http://www.cmake.org/
[GLAD]: https://github.com/Dav1dde/glad
[cmake_prefix]: https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html
