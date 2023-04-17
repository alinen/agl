#version 400

uniform sampler2D ScreenTexture1; 
uniform sampler2D ScreenTexture2; 
uniform vec2 Resolution;

out vec4 FragColor;

void main() {
  vec2 uv = gl_FragCoord.xy / Resolution.xy;

  vec3 color1 = texture(ScreenTexture1, uv).xyz;
  vec3 color2 = texture(ScreenTexture2, uv).xyz;

  FragColor = vec4(mix(color1, color2, 0.5), 1);
}
