#version 400

struct LightInfo
{
   vec4 position;  // 0 => directional light; 1 => point light
   vec3 color; 
};

struct MaterialInfo
{
   vec3 diffuse;
   vec3 ambient;
   vec3 specular;
   float shininess;
};

uniform LightInfo Light; 
uniform MaterialInfo Material;
uniform float Gamma;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

in vec4 position;
in vec3 normal;

out vec4 FragColor;

vec3 phongModel(in vec3 ePos, in vec3 eNormal)
{
  float w = Light.position.w; 
  vec3 s = normalize(Light.position.xyz - w * ePos);
  vec3 v = normalize(-ePos);
  vec3 h = normalize(reflect(s, eNormal));

  vec3 ambient = Light.color * Material.ambient;

  float angle = max( dot(s,eNormal), 0.0 ); 
  vec3 diffuse = angle * Light.color * Material.diffuse;

  float base = max(dot(h, -v), 0.0);
  vec3 spec = Light.color * Material.specular * pow(base, Material.shininess);

  return ambient + diffuse + spec;
}

void main()
{
  vec3 color = phongModel(position.xyz, normal);
  color = pow(color, vec3(Gamma));
  FragColor = vec4(color, 1.0);
}
