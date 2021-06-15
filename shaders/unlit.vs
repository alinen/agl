#version 400

layout (location = 0) in vec3 vPositions;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTextureCoords;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;

out vec3 color;

void main()
{
  color = 0.5 * (vNormals + 1.0);
  gl_Position = MVP * vec4(vPositions, 1.0);
}


