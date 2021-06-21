#version 400

in vec3 ReflectDir;
in vec3 RefractDir;
uniform samplerCube cubemap;
out vec4 FragColor;

void main()
{
	vec4 reflectColor = texture(cubemap, ReflectDir);
	vec4 refractColor = texture(cubemap, RefractDir);
	FragColor = mix(refractColor, reflectColor, 0.1);
}

