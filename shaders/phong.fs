#version 400

struct LightInfo
{
   vec4 position;
   vec3 color; 
};

struct Material
{
   vec3 Kd;
   vec3 Ka;
   vec3 Ks;
   float shininess;
};

uniform LightInfo uLight; 
uniform Material uMaterial;
uniform float uGamma;
uniform mat4 uMV;
uniform mat4 uMVP;
uniform mat3 uNMV;

in vec4 position;
in vec3 normal;

out vec4 FragColor;

vec3 phongModel(in vec3 ePos, in vec3 eNormal)
{
   float w = uLight.position.w; // 0 => directional light; 1 => point light
   vec3 s = normalize(uLight.position.xyz - w * ePos);
   vec3 v = normalize(-ePos);
   vec3 h = normalize(reflect(s, eNormal));

   vec3 ambient = uLight.color * uMaterial.Ka;

   float angle = max( dot(s,eNormal), 0.0 ); 
   vec3 diffuse = angle * uLight.color * uMaterial.Kd;

   float base = max(dot(h, -v), 0.0);
   vec3 spec = uLight.color * uMaterial.Ks * pow(base, uMaterial.shininess);

   return ambient + diffuse + spec;
}

void main()
{
   vec3 color = phongModel(position.xyz, normal);
   color = pow(color, vec3(uGamma));
   FragColor = vec4(color, 1.0);
   //FragColor = vec4(1, 0, 0, 1);
}

