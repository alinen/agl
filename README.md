# agl: A graphics library

Small, easy to use C++ library for 3D drawing, based on openGL.

## About

The goal of this project is to create an api that makes it easy to write 3D graphics demos in C++. This library supports my teaching and research (and so the api still changes frequently!). The design goals of this project are:

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

todo: image

## How to build

**Tools and dependencies**

To get started, you will need git and cmake. To build, you will also need to install GLEW, GLFW, and GLM.

* GLEW: Needed for accessing openGL 4.0 features
* GLFW: Implements our window and user interaction events, such as mouse and keyboard 
* GLM: Implements useful vector and matrix functionality for graphics

On windows, the dependencies are included in this repository. For macOS, you will need to install a subset of them separately.

*Windows*

On windows, you will need to have git and cmake installed.

* [Download and install git](https://git-scm.com/download/win).
* [Download and install cmake](https://github.com/Kitware/CMake/releases/download/v3.19.4/cmake-3.19.4-win64-x64.msi).

The dependencies, such as GLEW, GLFW, and GLM, are included in this repository under /external.

*macOS*

On macOS, you should have git and the clang C++ compiler from running `command xcode-select --install`. To install cmake, glew, and glfw, you can use brew. 

* `brew install cmake`
* `brew install glew`
* `brew install glfw3`

GLM is included with this repository but you can also install it [using the instructions here](http://macappstore.org/glm/). Other dependencies are included in /external.

**Build and run**

*Windows*

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

*macOS*

Open terminal to the directory containing this repository.

```
agl $ mkdir build
agl $ cd build
agl/build $ cmake ..
agl/build $ make
agl/build $ ../bin/sphere
```

## Demos
