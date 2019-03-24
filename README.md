# Composite Engine

### How do I get set up?

#### After Cloning

This repository requires [Git Large File Storage (Git LFS)](https://git-lfs.github.com/).

* Install [Git LFS](https://git-lfs.github.com/).
	* This must be done once for your machine.
* Run `git lfs install` in the repository directory.
	* This must be done once per repository clone.

#### Windows

* Install [Visual Studio 2019](https://www.visualstudio.com/downloads/).
	* Include `Desktop development with C++` and `Game development with C++`

#### Mac

* Install [CMake](https://cmake.org/download/).

* Install [Ninja](https://ninja-build.org).

* Install Xcode. Then:

```sh
# Install the command line tools.
xcode-select --install

# Switch to Xcode's command line tools.
sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer
```

#### Visual Studio Code

* Install the [C/C++ Extension from Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools).

* On Windows, open with Developer Command Prompt.

#### All Platforms

* Hit Play

### Dependencies

* SDL: https://www.libsdl.org/
	* Version in SDL.cmake
	* Versions: https://www.libsdl.org/download-2.0.php
	* Reference: http://wiki.libsdl.org/FrontPage

* GLEW: http://glew.sourceforge.net/
	* Version in GLEW.cmake
	* Repository: https://github.com/nigels-com/glew
	* Reference: See Above (sidebar)

* GLM: https://glm.g-truc.net/
	* 0.9.8.5: https://glm.g-truc.net/0.9.8/index.html
	* Reference: See Above (sidebar)

* FBX SDK: https://www.autodesk.com/products/fbx/overview
	* 2018.1.1: https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2018-1-1
	* Reference: http://help.autodesk.com/view/FBX/2018/ENU/

* stb_image: https://github.com/nothings/stb
	* 2.16: https://github.com/nothings/stb/blob/9d9f75eb682dd98b34de08bb5c489c6c561c9fa6/stb_image.h
	* Reference: See File

* Chromium Embedded Framework: https://bitbucket.org/chromiumembedded/cef/src/master/
	* Version in CEF.cmake
	* Build Index: http://opensource.spotify.com/cefbuilds/index.html
	* Reference: https://bitbucket.org/chromiumembedded/cef/wiki/Home

* RapidJSON: https://github.com/Tencent/rapidjson
	* 67fac85 (master): https://github.com/Tencent/rapidjson/tree/67fac85e96220f69076121d569abd15471abb6bf
	* Reference: http://rapidjson.org/

### Resources

* DirectX:
	* https://www.3dgep.com/introduction-to-directx-11/
	* http://www.walkerb.net/blog/dx-4/

* OpenGL:
	* References:
		* https://www.khronos.org/opengl/wiki/Main_Page
			* https://www.khronos.org/opengl/wiki/Getting_Started
			* https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
			* https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
			* https://www.khronos.org/opengl/wiki/Related_toolkits_and_APIs
		* https://www.opengl.org/registry/
			* https://www.khronos.org/registry/OpenGL/index_gl.php
		* https://www.opengl.org/archives/resources/faq/technical/gettingstarted.htm (archived)
		* https://en.wikipedia.org/wiki/OpenGL
		* https://en.wikipedia.org/wiki/OpenGL_ES
		* http://ogl-samples.g-truc.net/ (click Home button)
	* Tutorials:
		* https://www.3dgep.com/introduction-to-opengl-and-glsl/
		* http://www.opengl-tutorial.org/
		* https://learnopengl.com/
		* https://sites.google.com/site/opengltutorialsbyaks/
			* https://sites.google.com/site/opengltutorialsbyaks/introduction-to-opengl-3-2---tutorial-01
		* https://en.wikibooks.org/wiki/OpenGL_Programming
	* Function Loading and GLEW-specific:
		* https://stackoverflow.com/questions/3933027/how-to-get-the-gl-library-headers
		* https://stackoverflow.com/questions/14413/using-opengl-extensions-on-windows
		* https://stackoverflow.com/questions/17370/using-glew-to-use-opengl-extensions-under-windows
		* https://gamedev.stackexchange.com/questions/112182/what-is-glew-and-how-does-it-work
		* https://www.opengl.org/discussion_boards/showthread.php/175661-Using-OpenGL-without-GLEW
	* Apple:
		* https://support.apple.com/en-us/HT202823
		* https://developer.apple.com/library/content/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/OpenGLESontheiPhone/OpenGLESontheiPhone.html

* SDL and OpenGL Tutorials: http://lazyfoo.net/

* Lang's FBX SDK Guide: https://www.gamedev.net/articles/programming/graphics/how-to-work-with-fbx-sdk-r3582/
* C++ Skeletal Animation Library: http://guillaumeblanc.github.io/ozz-animation/
	* https://github.com/guillaumeblanc/ozz-animation

* Models and Animations: https://www.mixamo.com/

* NVIDIA Nsight: http://www.nvidia.com/object/nsight.html
	* Reference: http://docs.nvidia.com/nsight-visual-studio-edition/5.4/Nsight_Visual_Studio_Edition_User_Guide.htm
