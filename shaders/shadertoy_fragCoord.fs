#version 400

uniform vec2 iResolution;
uniform float iGlobalTime;

out vec4 FragColor;
void main() {
  vec2 p = gl_FragCoord.xy / iResolution.xx;
  FragColor = vec4(p.xy,0,1);
}
