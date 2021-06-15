#version 400

layout (location = 0) in vec3 vPositions;
uniform mat4 MVP;

void main()
{
  gl_Position = MVP * vec4(vPositions, 1.0);
}


