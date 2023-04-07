#version 400

uniform samplerCube cubemap;
uniform vec3 etaRatio;
uniform float fresnel;
uniform float fresnelPower;

in vec3 eyePosition;
in vec3 position;
in vec3 normal;
in vec2 uv;

out vec4 FragColor;

void main()
{
	vec3 v = position - eyePosition;
	vec3 refractDirR = refract(v, normal, etaRatio.r);
	vec3 refractDirG = refract(v, normal, etaRatio.g);
	vec3 refractDirB = refract(v, normal, etaRatio.b);

	vec3 reflectDir = reflect(v, normal);
	vec4 reflectColor = texture(cubemap, reflectDir);

	float refractColorR = texture(cubemap, refractDirR).r;
	float refractColorG = texture(cubemap, refractDirG).g;
	float refractColorB = texture(cubemap, refractDirB).b;
	vec4 refractColor = vec4(refractColorR, refractColorG, refractColorB, 1.0);

	float reflectivity = fresnel + (1 - fresnel) * pow(1 + dot(v, normal), fresnelPower);
	reflectivity = max(0, min(1, reflectivity));
	FragColor = mix(reflectColor, refractColor, reflectivity);
}

