#version 400

layout (location = 0) in vec3 vPositions;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTextureCoords;

struct LightInfo {
  vec4 position;  // 0 => directional light; 1 => point light
  vec3 color; 
};

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ViewMatrix;
uniform mat4 MVP;
uniform LightInfo Light; 

out vec4 position;
out vec4 lightpos;
out vec3 normal;
out vec2 uv;

void main()
{
  position = ModelViewMatrix * vec4( vPositions, 1.0);
  lightpos = ViewMatrix * Light.position;
  normal = NormalMatrix * vNormals;
  uv = vTextureCoords;
  gl_Position = MVP * vec4(vPositions, 1.0);
}


