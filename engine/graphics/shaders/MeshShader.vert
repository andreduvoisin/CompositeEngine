#version 410

uniform mat4 projectionViewModel;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextureCoordinate;

out vec2 textureCoordinate;

void main()
{
	gl_Position = projectionViewModel * vec4(vertexPosition, 1.0);
	textureCoordinate = vertexTextureCoordinate;
}
