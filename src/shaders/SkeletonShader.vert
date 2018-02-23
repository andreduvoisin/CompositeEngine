#version 410

uniform mat4 projectionViewModel;
uniform samplerBuffer palette;

in vec3 vertexPosition;
in vec3 vertexColor;
in int jointIndex;

out vec3 color;

mat4 FetchJointTransform(in int jointIndex)
{
	return mat4(
		texelFetch(palette, jointIndex * 4),
		texelFetch(palette, jointIndex * 4 + 1),
		texelFetch(palette, jointIndex * 4 + 2),
		texelFetch(palette, jointIndex * 4 + 3));
}

void main()
{
	vec4 skinnedPosition = FetchJointTransform(jointIndex) * vec4(vertexPosition, 1.0);
	gl_Position = projectionViewModel * skinnedPosition;
	color = vertexColor;
}
