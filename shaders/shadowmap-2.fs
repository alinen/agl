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

uniform sampler2DShadow shadowMap;
uniform sampler2D mainTexture;
uniform MaterialInfo Material;
uniform LightInfo Light; 
uniform bool PCF;
uniform bool useShadows;

in vec4 shadowCoord;
in vec3 lightdir;
in vec3 viewdir;
in vec3 normal;
in vec2 uv;

layout (location = 0) out vec4 FragColor;

vec3 phongModel(in vec3 eNormal) {
  vec3 s = lightdir;
  vec3 v = viewdir;
  vec3 h = normalize(reflect(s, eNormal));

  vec3 ambient = Light.color * Material.ambient;

  float angle = max( dot(s,eNormal), 0.0 ); 
  vec3 diffuse = angle * Light.color * Material.diffuse;

  float base = max(dot(h, -v), 0.0);
  vec3 spec = Light.color * Material.specular * pow(base, Material.shininess);

  float inShadow = 0.0;
  if (PCF) {
    inShadow += textureProjOffset(shadowMap, shadowCoord, ivec2(-1,-1));
    inShadow += textureProjOffset(shadowMap, shadowCoord, ivec2( 1,-1));
    inShadow += textureProjOffset(shadowMap, shadowCoord, ivec2(-1, 1));
    inShadow += textureProjOffset(shadowMap, shadowCoord, ivec2( 1, 1));
    inShadow = inShadow * 0.25;
  }
  else {
    inShadow = textureProj(shadowMap, shadowCoord);
  }

  vec3 color; 
  if (useShadows) {
    color = ambient + inShadow * (diffuse + spec);
  }
  else {
    color = ambient + diffuse + spec;
  }
  return color;
}

void main() {
  vec3 color = phongModel(normal);
  color = pow(color, vec3(1.2));
  FragColor = vec4(color, 1.0);
}
