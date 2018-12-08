#version 410

uniform mat4 projectionViewModel;

in vec3 vertexPosition;

void main()
{
	gl_Position = projectionViewModel * vec4(vertexPosition, 1.0);
}
