#version 410

in vec2 vertexPosition;
in vec2 vertexTextureCoordinate;

out vec2 textureCoordinate;

void main()
{
	gl_Position = vec4(vertexPosition, 0, 1);
	textureCoordinate = vertexTextureCoordinate;
}
