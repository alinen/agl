#version 400

struct LightInfo {
  vec4 position;  // 0 => directional light; 1 => point light
  vec3 color; 
};

struct MaterialInfo {
  vec3 diffuse;
  vec3 ambient;
  vec3 specular;
  float shininess;
};

uniform sampler2D texture;
uniform sampler2D normalmap;

uniform LightInfo Light; 
uniform MaterialInfo Material;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform bool useNormalMap;

in vec4 position;
in vec4 lightpos;
in vec3 normal;
in vec2 uv;

out vec4 FragColor;

vec3 phongModel(in vec3 ePos, in vec3 eNormal) {
  vec3 L = normalize(lightpos.xyz - lightpos.w * ePos);
  vec3 v = normalize(-ePos);
  vec3 r = normalize(reflect(v, eNormal));

  vec3 ambient = Light.color * Material.ambient;

  float angle = max( dot(L,eNormal), 0.0 ); 
  vec3 diffuse = angle * Light.color * Material.diffuse;

  vec3 mainColor = texture(texture, uv).rgb;
  vec3 color = mainColor*(ambient + diffuse);

  float base = max(dot(r, -v), 0.0);
  vec3 spec = Light.color * Material.specular * pow(base, Material.shininess);

  return color;
}

void main() {
  vec3 texNormal = normalize(2*(texture(normalmap, uv).xyz-0.5f));
  vec3 color = phongModel(position.xyz, useNormalMap? texNormal : normal);
  FragColor = vec4(color, 1.0);
}
