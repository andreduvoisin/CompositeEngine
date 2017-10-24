#version 410

uniform sampler2D ourTexture;

in vec2 texCoord;

out vec4 frag_colour;

void main() {
	frag_colour = texture(ourTexture, texCoord);
}
