#version 400

layout (location = 0) in vec3 vPosition;
out vec3 ReflectDir;
uniform mat4 MVP;

void main()
{
   ReflectDir = vPosition;
   gl_Position = MVP * vec4(vPosition, 1.0);
}
