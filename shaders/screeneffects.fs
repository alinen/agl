#version 400

uniform sampler2D ScreenTexture; 
uniform float GlobalTime;
uniform vec2 Resolution;
uniform int EffectType;

out vec4 FragColor;

vec4 pixelate(vec2 uv) {
  uv = 0.01 * floor(uv/0.01);
  return texture(ScreenTexture, uv);
}

vec4 invert(vec2 uv) {
  vec3 color = texture(ScreenTexture, uv).xyz;
  return vec4(vec3(1) - color, 1);
}

void main() {
  vec2 uv = gl_FragCoord.xy / Resolution.xy;

  vec4 color;
  if (EffectType == 0) color = texture(ScreenTexture, uv);
  else if (EffectType == 1) color = pixelate(uv); 
  else if (EffectType == 2) color = invert(uv);

  FragColor = color;
}
