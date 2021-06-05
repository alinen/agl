#version 400

in vec2 uv;
in vec4 color;

uniform bool isLine;
uniform sampler2D image;

out vec4 FragColor;

void main()
{
   if (isLine)
   {
      FragColor = color;
   }
   else
   {
      FragColor = color * texture(image, uv);
   }
}
