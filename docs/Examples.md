# Examples

## Custom meshes

### Undulating mesh

To create a dynamic triangle mesh, override `TriangleMesh` and call `setIsDynamic`. Below, we override the `Plane` primitive. The full code is [here](https://github.com/alinen/agl/blob/main/examples/undulate.cpp).

<video controls>
  <source src="movie.mp4" type="video/mov">
  Your browser does not support the video tag.
</video> 

### Attractor

The following example creates a `PointMesh` to draw a atrsnage attractor. The full code is [here](https://github.com/alinen/agl/blob/main/examples/attractor.cpp).

## Textures

### Dynamic texture

The colors of the texture change when the user presses the 'up' key. The full code is [here](https://github.com/alinen/agl/blob/main/examples/texture.cpp).

### Procedural texture

We can create textures directly from code. Below we draw a checker pattern using the `Image` class. The full code is [here](https://github.com/alinen/agl/blob/main/examples/checker.cpp).

### Sprites

The following shows how to use multiple textures using `texture` and `loadTexture`. If the same slot id is used for both textures, the new texture replaces the old one. If multiple textures are needed, they should each have their own slot id. The full code is [here](https://github.com/alinen/agl/blob/main/examples/sprites.cpp).

### Cubemap

The follow code shows how to load a cubemap using `texture` and `loadCubemap`. The full code is [here]().

## Built-in primitives

The following example shows how to enable fog, set colors, and draw different primitives. The full code is [here](https://github.com/alinen/agl/blob/main/examples/shapes.cpp).

## Shadertoy

You can write fragment programs that do ray tracing and ray marching -- like in [shadertoy](https://www.shadertoy.com). Below we draw a quad the faces the camera and has the same dimensions as the window. The scene is then drawn in [glsl-raymarch.fs](https://github.com/alinen/agl/blob/main/shaders/glsl-raymarch.fs). The application code is [here](https://github.com/alinen/agl/blob/main/shaders/shadertoy_raymarch_csg.fs).
