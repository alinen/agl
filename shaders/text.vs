#version 400

layout (location = 0) in vec2 vPositions;
layout (location = 1) in vec2 vUVs;
layout (location = 2) in vec4 vColor;

uniform mat4 MVP;
out vec2 uv;
out vec4 color;

void main()
{
  color = clamp(vColor/255.0, vec4(0), vec4(1));
  uv = vUVs;
  gl_Position = MVP * vec4(vPositions, 0.0, 1.0);
}
