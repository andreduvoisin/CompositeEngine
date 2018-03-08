#version 410

uniform sampler2D diffuseTexture;

in vec2 textureCoordinate;

out vec4 fragmentColor;

void main()
{
	fragmentColor = texture(diffuseTexture, textureCoordinate);
}
