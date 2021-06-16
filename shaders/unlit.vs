#version 400

layout (location = 0) in vec3 vPositions;

struct MaterialInfo {
  vec4 color;
};

uniform mat4 MVP;
uniform MaterialInfo Material;

out vec4 color;

void main()
{
  color = Material.color;
  gl_Position = MVP * vec4(vPositions, 1.0);
}
