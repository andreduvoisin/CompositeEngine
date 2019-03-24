#version 410

uniform mat4 projectionViewModel;

layout(location = 0) in vec3 vertexPosition;

void main()
{
	gl_Position = projectionViewModel * vec4(vertexPosition, 1.0);
}
