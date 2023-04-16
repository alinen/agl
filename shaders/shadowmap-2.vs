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
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ShadowMatrix;
uniform mat4 MVP;
uniform LightInfo Light;

out vec4 shadowCoord;
out vec3 lightdir;
out vec3 viewdir;
out vec3 normal;
out vec2 uv;

void main()
{
  vec4 position = ModelViewMatrix * vec4( vPositions, 1.0);
  normal = normalize( NormalMatrix * vNormals);
  uv = vTextureCoords;

  vec4 lightpos = ViewMatrix * vec4(Light.position.xyz, 1.0);
  lightdir = normalize(lightpos.xyz - Light.position.w * position.xyz);
  viewdir = normalize(-position.xyz);
  shadowCoord = ShadowMatrix * ModelMatrix * vec4( vPositions, 1.0);

  gl_Position = MVP * vec4(vPositions, 1.0);
}
