# Getting Started

The main classes implemented in this library are 

* **[Window](classagl_1_1_window.html)**: Manages user input, window properties, and main application loop
* **Renderer**: Manages drawing and shaders. Supports a phong-based default shader
* **Image**: Loading, manipulating, and saving images to use as textures
* **Mesh**: Manages objects composed of large numbers of vertices. You can use an existing primitive or define your own.

## How to set up your scene

## How to use the default camera

## How to respond to events


## How to draw shapes

* Built-in shapes
* User-defined shapes
* Positioning using the matrix stack

## How to work with shaders

* Using the built-in shaders (default layouts and uniform parameters)

* Defining your own shaders

## How to load and use textures

## Common problems

### I have a black screen. What's wrong?

* Check your projection and camera properties. Make sure that any objects you define are located inside the view volume.
* Try drawing a simple shape
* Try drawing with the unlit shader
* Make sure the the object color and background colors are different
* If your colors have transparency, make sure that alpha is not 0.0


## Known issues

* Only RGBA images are currently supported

