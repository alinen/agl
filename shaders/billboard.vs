#version 400

layout (location = 0) in vec3 vPos;

uniform vec3 uCameraPos;
uniform float uSize;
uniform float uRot;
uniform vec3 uOffset;
uniform vec4 uColor;
uniform mat4 uVP;
uniform bool isLine;

out vec4 color;
out vec2 uv;

void main()
{
   if (isLine)
   {
      color = uColor;
      gl_Position = uVP * vec4(vPos, 1.0); 
   }
   else
   {
      color = uColor;
      uv = vPos.xy;

      vec3 z = normalize(uCameraPos - uOffset);
      vec3 x = normalize(cross(vec3(0,1,0), z));
      vec3 y = normalize(cross(z, x));
      mat3 R = mat3(x, y, z);

      vec3 eyePos = R * uSize * (vPos - vec3(0.5, 0.5, 0.0)) + uOffset;
      gl_Position = uVP * vec4(eyePos, 1.0); 
   }
}
