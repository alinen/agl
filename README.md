# agl

A graphics library. Small, easy to use C++ library for 3D drawing, based on openGL.

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

Dependencies:

* stb
* glfw
* glew
* glm

**MacOS**

**Windows**

**Ubuntu**

## Demos/Experriments
