# A graphics library

Small, easy to use C++ library for 3D drawing, based on openGL.

## About

The goal of this project is to create an api that makes it easy to write 3D
graphics demos in C++. This library supports my teaching and research (and so
the api still changes frequently!). The design goals of this project are:

* To make common tasks easy, e.g. drawing primitives, loading textures, defining shaders
* To make less common tasks possible
* To support fast prototyping and experimentation

For example, the following program draws a sphere with phong shading

```
#include "agl/window.h"

class MyWindow : public agl::Window {
  void draw() {
    renderer.sphere();
  }
};

int main() {
  MyWindow window;
  window.run();
}
```

![](docs/img/sphere.cpp.screenshot.png)

## How to build

### Tools and dependencies

To get started, you will need a C++ development environment, git, cmake. To
build, you will also need to several dependencies.

* GLEW: Needed for accessing openGL 4.0 features
* GLFW: Implements our window and user interaction events, such as mouse and keyboard 
* GLM: Implements useful vector and matrix functionality for graphics
* STB: Needed for loading images
* GLTF: Needed for loading models

**Windows**

On windows, you will need to have a C++ build environment, git and cmake installed.
If you do not have a C++ compiler on windows, you can use the community version of visual studio. 

* Download and install [Visual Studio Community 2019](https://visualstudio.microsoft.com/vs/community/).

*Important!!* During installation, select: Desktop development with C++ 

<img src="https://raw.githubusercontent.com/BrynMawr-CS312-2021/cplusplus-beginners/b7d59e351b6c491ca96020273d9a5cb2a59427d8/InstallMSVC.png" alt="InstallOptions" width="200"/>

* [Download and install git](https://git-scm.com/download/win).
* [Download and install cmake](https://github.com/Kitware/CMake/releases/download/v3.19.4/cmake-3.19.4-win64-x64.msi).

The dependencies, such as GLEW, GLFW, and GLM, are included in this repository under /external.

**macOS**

On macOS, you should have git and a C++ compiler from running `command xcode-select --install`. To install cmake, glew, and glfw, you can use brew. 

* `brew install cmake`
* `brew install glew`
* `brew install glfw3`

GLM is included with this repository but you can also install it [using the instructions here](http://macappstore.org/glm/). Other dependencies are included in /external.

**Ubuntu**

On linux, you can install many of the necessary tools and dependencies using `apt-get`.

* `sudo apt-get install cmake`
* `sudo apt-get install git`
* `brew install glfw3`

todo

### Compiling and running 

**Windows**

On windows, use cmake to build either makefiles or a visual studio project. And then 
build from there.

From terminal (either git bash or powershell):

```
agl $ mkdir build
agl $ cd build
agl/build $ cmake -G "Visual Studio 16 2019" ..
agl/build $ start AGL.sln
```

After building all source, you can run either from visual studio or the command line:

```
agl/build $ ../bin/Debug/sphere.exe
```

**macOS/Ubuntu**

Open terminal to the directory containing this repository.

```
agl $ mkdir build
agl $ cd build
agl/build $ cmake ..
agl/build $ make
agl/build $ ../bin/sphere
```

### Docs

Read the docs [here](https://alinen.github.io/agl/html/index.html).

### References 

* [Doxygen Awesome CSS theme](https://jothepro.github.io/doxygen-awesome-css/index.html)
* [OpenGL](https://www.khronos.org/registry/OpenGL-Refpages/gl4/)
* [GLFW](https://www.glfw.org/)
* [GLM](https://glm.g-truc.net/0.9.5/api/index.html)
* [STB](https://github.com/nothings/stb)
* [GLTF](https://www.khronos.org/gltf/)
* [cpplint](https://github.com/cpplint/cpplint)
* [OpenGL shader cookbook](https://github.com/PacktPublishing/OpenGL-4-Shading-Language-Cookbook-Third-Edition)
