#version 400

in vec4 clipPosition;
out vec4 FragColor;

void main() {
  float d = 0.5 * (clipPosition.z/clipPosition.w + 1);
  FragColor = vec4(d, d, d, 1.0);
}
