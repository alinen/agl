#version 400

layout (location = 0) in vec3 vPos;
out vec3 ReflectDir;
uniform mat4 uVP;

void main()
{
   ReflectDir = vPos;
   gl_Position = uVP * vec4(vPos, 1.0);
}
