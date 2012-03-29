Example code from the [OpenGL 4.0 Shading Language Cookbook][cookbook]
=========================================================

This is the example programs from the [OpenGL 4.0 Shading Language
Cookbook][cookbook], by David Wolff.  The source code has been updated to work
with MS Visual Studio, and no longer requires Qt.

Requirements
-------------
To use these examples, you'll need the following:

* The latest version of the Unofficial OpenGL Software Development Kit (GLSDK), 
  available at <http://glsdk.sourceforge.net>.
* You'll also need the clever and useful tool [premake][].
* In order to use the examples in Chapter 8, you'll need at least version 
  0.9.3 of [GLM][].  Unfortunately, at this time, the
  [GLSDK][] includes version 0.9.2.x of [GLM][].  However, it is easy to 
  substitute a newer version (see below).
  
Compiling the examples
----------------------
These examples have been fully tested with Visual Studio 2010.  I hope to have
them working with other compilers soon.  Your help is welcome!  Fork 
[this project][ghcookbook] on github.

1.  Install the [GLSDK][] by following the instructions on their [web site][glsdk].
    In the process, you'll also install [premake][].
2.  If you want to use the examples in Chapter 8, install the latest version
    of [GLM][] into the GLSDK.  You can do so by removing the `glm` directory
    and replacing it with the contents of the GLM zip file.  Make sure to change
    the directory name to just `glm`.
3.  Download the example code from [github][ghcookbook].  Unzip to an appropriate
    directory.
4.  Edit the `premake4.lua` file to point to your GLSDK's `links.lua` file.
5.  Run `premake4 vs2010` to generate the project and solution files.
6.  Open the solution in Visual Studio, and compile!

Any problems, create an issue on [github][ghcookbook].

Changes from the book
------------------------
At the time the book was written, I thought that using Qt would make the examples
quite portable and easy to compile within Qt Creator.  Unfortunately, Qt Creator
has changed since the time of the writing of the book.  It has become more 
unwieldy to install, and in my experience has become extremely slow to boot.
In addition, it is not widely used.

Secondly, since the time of the book's writing, I have experienced problems
with GLEW under a 4.0 core profile.  When doing some research it appears that
there are some fundemental issues with GLEW and core profiles.  

Due to the above issues, and to make the code more available and hopefully 
easier to use, I have ported the code over
to use GLLoad and FreeGLUT via the GLSDK.  The code has been fully tested and 
compiles in VisualStudio 2010, and will soon (hopefully) be tested with the 
GNU compiler chain under Linux.

The major changes are listed below:

* Dropped dependence on Qt.
* Uses FreeGLUT, GLM, GLLoad, and GLImage via the [GLSDK][].
* Dropped dependence on libnoise for Chapter 8 examples.  Instead, uses the noise
  functions available in [GLM][] > 0.9.3.

TODO
--------------
* Currently, the noise examples (Chapter 8) don't look quite right due to the move
  to GLM.  Fix noise examples (chapter 8) to look better.

[GLM]: http://glm.g-truc.net
[GLSDK]:  http://glsdk.sourceforge.net
[premake]:  http://industriousone.com/premake
[ghcookbook]:  http://github.com/daw42/glslcookbook
[cookbook]: http://www.packtpub.com/opengl-4-0-shading-language-cookbook/book
