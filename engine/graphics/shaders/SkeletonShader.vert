#version 410

uniform mat4 projectionViewModel;
uniform samplerBuffer palette;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in uint jointIndex;

out vec3 color;

mat4 FetchJointTransform(in uint jointIndex)
{
	return mat4(
		texelFetch(palette, int(jointIndex) * 4),
		texelFetch(palette, int(jointIndex) * 4 + 1),
		texelFetch(palette, int(jointIndex) * 4 + 2),
		texelFetch(palette, int(jointIndex) * 4 + 3));
}

void main()
{
	vec4 skinnedPosition = FetchJointTransform(jointIndex) * vec4(vertexPosition, 1.0);
	gl_Position = projectionViewModel * skinnedPosition;
	color = vertexColor;
}
