#version 400

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

uniform mat3 ModelInverseTransposeMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 MVP;

out vec3 eyePosition;
out vec3 position;
out vec3 normal;
out vec2 uv;

void main()
{
  // Use global coordinates in the fragment shader!
  eyePosition = vec3(inverse(ViewMatrix)[3]); 
  position = vec3(ModelMatrix * vec4( vPosition, 1.0));
  normal = normalize( ModelInverseTransposeMatrix * vNormal);
  uv = vUV;

  gl_Position = MVP * vec4(vPosition, 1.0);
}
