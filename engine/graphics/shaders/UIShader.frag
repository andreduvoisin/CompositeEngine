#version 410

uniform sampler2D uiTexture;

in vec2 textureCoordinate;

out vec4 fragmentColor;

void main()
{
	fragmentColor = texture(uiTexture, textureCoordinate);
}
