#version 400

in vec4 color;
in vec2 uv;

uniform sampler2D fontTexture;
out vec4 FragColor;

void main()
{
  vec4 c = color * texture(fontTexture, uv);
  FragColor = c; //vec4(1,0,0,1);
}
