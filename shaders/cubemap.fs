#version 400

in vec3 ReflectDir;
uniform samplerCube cubemap;
out vec4 FragColor;

void main()
{
	vec4 cubeMapColor = texture(cubemap, ReflectDir);
	FragColor = cubeMapColor;
}

