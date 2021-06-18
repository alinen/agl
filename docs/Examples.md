# Examples

## Custom meshes

### Undulating mesh

To create a dynamic triangle mesh, override `TriangleMesh` and call `setIsDynamic`. Below, we override the `Plane` primitive. The full code is [here](https://github.com/alinen/agl/blob/main/examples/undulate.cpp).

https://user-images.githubusercontent.com/259657/122494550-94f54980-cfb7-11eb-8111-b4bc5c920d00.mp4

### Attractor

The following example creates a `PointMesh` to draw a atrsnage attractor. The full code is [here](https://github.com/alinen/agl/blob/main/examples/attractor.cpp).

https://user-images.githubusercontent.com/259657/122494655-9de61b00-cfb7-11eb-91b6-ea922be4873e.mp4

## Textures

### Dynamic texture

The colors of the texture change when the user presses the 'up' key. The full code is [here](https://github.com/alinen/agl/blob/main/examples/texture.cpp).

https://user-images.githubusercontent.com/259657/122494807-a6d6ec80-cfb7-11eb-976b-18b5f8abfe9a.mp4

### Procedural texture

We can create textures directly from code. Below we draw a checker pattern using the `Image` class. The full code is [here](https://github.com/alinen/agl/blob/main/examples/checker.cpp).

<img width="487" alt="checker" src="https://user-images.githubusercontent.com/259657/122494946-ae969100-cfb7-11eb-86e6-2b6830570594.png">

### Sprites

The following shows how to use multiple textures using `texture` and `loadTexture`. If the same slot id is used for both textures, the new texture replaces the old one. If multiple textures are needed, they should each have their own slot id. The full code is [here](https://github.com/alinen/agl/blob/main/examples/sprites.cpp).

### Cubemap

The follow code shows how to load a cubemap using `texture` and `loadCubemap`. The full code is [here]().

## Built-in primitives

The following example shows how to enable fog, set colors, and draw different primitives. The full code is [here](https://github.com/alinen/agl/blob/main/examples/shapes.cpp).

https://user-images.githubusercontent.com/259657/122495146-b81ff900-cfb7-11eb-88d3-7f12ca2a5de4.mp4

## Shadertoy

You can write fragment programs that do ray tracing and ray marching -- like in [shadertoy](https://www.shadertoy.com). Below we draw a quad the faces the camera and has the same dimensions as the window. The scene is then drawn in [glsl-raymarch.fs](https://github.com/alinen/agl/blob/main/shaders/glsl-raymarch.fs). The application code is [here](https://github.com/alinen/agl/blob/main/shaders/shadertoy_raymarch_csg.fs).

https://user-images.githubusercontent.com/259657/122495250-bd7d4380-cfb7-11eb-86a1-2ba2fb181059.mp4
