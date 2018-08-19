#version 410

uniform vec2 viewport;

in vec2 vertexPosition;
in vec2 vertexTextureCoordinate;

out vec2 textureCoordinate;

void main()
{
	gl_Position = vec4(2*vertexPosition.xy / viewport.xy - 1, 0, 1);
	textureCoordinate = vertexTextureCoordinate;
}
