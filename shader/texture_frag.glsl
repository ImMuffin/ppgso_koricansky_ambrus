#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// (optional) Transparency
uniform float Transparency = 1;

// (optional) LightColor
uniform vec3 LightColor;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 texCoord;

// The final color
out vec4 FragmentColor;

void main() {
  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);
  FragmentColor.r *= 0;
  FragmentColor.g *= 0;
  FragmentColor.b *= 0;
  FragmentColor.r += LightColor[0];
  FragmentColor.g += LightColor[1];
  FragmentColor.b += LightColor[2];
  FragmentColor.a = Transparency;
}
