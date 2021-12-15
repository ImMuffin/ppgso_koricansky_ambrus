#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection;

// (optional) Transparency
uniform float Transparency = 1;

// (optional) Texture offset
uniform vec2 TextureOffset;

// texture movement
uniform float TimeOffset;

// The vertex shader will feed this input
in vec2 texCoord;

// The final color
out vec4 FragmentColor;

// Wordspace normal passed from vertex shader
in vec4 normal;

void main() {
  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  vec2 texcoord = texCoord;
  texcoord.x += sin(texcoord.y * 4*2*3.14159 + TimeOffset) / 100;
  float diffuse = max(dot(normal, vec4(normalize(LightDirection), 1.0f)), 0.0f);
  FragmentColor = texture(Texture, vec2(texcoord.x, 1.0 - texcoord.y) + TextureOffset) * diffuse;
  FragmentColor.a = Transparency;
}
