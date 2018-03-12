#version 410

uniform mat4 projectionViewModel;

in vec3 vertexPosition;
in vec2 vertexTextureCoordinate;

out vec2 textureCoordinate;

void main()
{
	gl_Position = projectionViewModel * vec4(vertexPosition, 1.0);
	textureCoordinate = vertexTextureCoordinate;
}
