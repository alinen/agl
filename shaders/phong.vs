#version 400

layout (location = 0) in vec3 vPositions;
layout (location = 1) in vec3 vNormals;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;

out vec4 position;
out vec3 normal;

void main()
{
  position = ModelViewMatrix * vec4( vPositions, 1.0);
  normal = normalize( NormalMatrix * vNormals);
  gl_Position = MVP * vec4(vPositions, 1.0);
}


