#version 400

layout (location = 0) in vec3 vPositions;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTextureCoords;

uniform mat4 MVP;
out vec4 clipPosition;

void main()
{
  clipPosition = MVP * vec4(vPositions, 1.0);
  gl_Position = MVP * vec4(vPositions, 1.0);
}
