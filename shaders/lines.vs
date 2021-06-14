#version 400

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;

uniform mat4 MVP;
out vec4 color;

void main()
{
  color = vec4(vColor, 1);
  gl_Position = MVP * vec4(vPosition, 1.0); 
}
