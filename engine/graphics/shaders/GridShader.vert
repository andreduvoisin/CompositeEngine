#version 410

uniform mat4 projectionViewModel;

in vec3 vertexPosition;
in vec3 vertexColor;

out vec3 color;

void main()
{
	gl_Position = projectionViewModel * vec4(vertexPosition, 1.0);
	color = vertexColor;
}
