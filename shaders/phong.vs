#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;

uniform mat3 uNMV;
uniform mat4 uMV;
uniform mat4 uMVP;

out vec4 position;
out vec3 normal;

void main()
{
   position = uMV * vec4( vPos, 1.0);
   normal = normalize( uNMV * vNor);
   gl_Position = uMVP * vec4(vPos, 1.0);
}


