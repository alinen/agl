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

struct FogInfo {
  vec3 color;
  float minDist;
  float maxDist;
  bool enabled;
};

struct TextureInfo {
  sampler2D texture;
  bool enabled;
  vec2 tile;
  vec2 offset;
};

uniform LightInfo Light; 
uniform MaterialInfo Material;
uniform FogInfo Fog;
uniform float Gamma;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform TextureInfo DetailTexture;
uniform TextureInfo MainTexture;
uniform bool HasUV;

in vec4 position;
in vec3 normal;
in vec2 uv;

out vec4 FragColor;

vec3 phongModel(in vec3 ePos, in vec3 eNormal) {
  float w = Light.position.w; 
  vec3 s = normalize(Light.position.xyz - w * ePos);
  vec3 v = normalize(-ePos);
  vec3 h = normalize(reflect(s, eNormal));

  vec3 ambient = Light.color * Material.ambient;

  float angle = max( dot(s,eNormal), 0.0 ); 
  vec3 diffuse = angle * Light.color * Material.diffuse;

  vec3 color = ambient + diffuse;
  if (HasUV) {
    vec4 texColor = vec4(1.0f);
    if (MainTexture.enabled) {
      vec2 muv = uv * MainTexture.tile + MainTexture.offset;
      vec4 mainColor = texture(MainTexture.texture, muv);
      texColor = mainColor * texColor;
    }

    if (DetailTexture.enabled) {
      vec2 duv = uv * DetailTexture.tile + DetailTexture.offset;
      vec4 detailColor = texture(DetailTexture.texture, duv);
      texColor = mix(texColor, detailColor, detailColor.a);
    }

    color = color * texColor.xyz;
  }

  float base = max(dot(h, -v), 0.0);
  vec3 spec = Light.color * Material.specular * pow(base, Material.shininess);

  return color + spec;
}

void main() {
  vec3 color = phongModel(position.xyz, normal);
  color = pow(color, vec3(Gamma));

  if (Fog.enabled) {
    float u = (length(position.xyz) - Fog.minDist)/(Fog.maxDist - Fog.minDist);
    color = mix(color, Fog.color, clamp(u, 0.0, 1.0));
  }
  FragColor = vec4(color, 1.0);
}
