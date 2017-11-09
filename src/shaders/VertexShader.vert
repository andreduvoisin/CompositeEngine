#version 410

uniform mat4 projectionViewModel;
uniform samplerBuffer palette;

in vec3 vertexPosition;
in vec2 vertexTextureCoordinate;
in int jointIndices[4];
in float jointWeights[4];

out vec2 textureCoordinate;

vec4 CalculateWeightedPosition(in mat4 jointTransform, in float jointWeight)
{
	return (jointTransform * vec4(vertexPosition, 1.0)) * jointWeight;
}

mat4 FetchJointTransform(in int jointIndex)
{
	return mat4(
		texelFetch(palette, jointIndex * 4),
		texelFetch(palette, jointIndex * 4 + 1),
		texelFetch(palette, jointIndex * 4 + 2),
		texelFetch(palette, jointIndex * 4 + 3));
}

vec4 WeightedPositionForJoint(in int jointIndex, in float jointWeight)
{
	mat4 jointTransform = FetchJointTransform(jointIndex);
	return CalculateWeightedPosition(jointTransform, jointWeight);
}

vec4 PositionForWeightIndex(in int index)
{
	return WeightedPositionForJoint(jointIndices[index], jointWeights[index]);
}

vec4 CalculateSkinnedPosition()
{
	vec4 skinnedPosition = vec4(0.0);
	skinnedPosition += PositionForWeightIndex(0);
	skinnedPosition += PositionForWeightIndex(1);
	skinnedPosition += PositionForWeightIndex(2);
	skinnedPosition += PositionForWeightIndex(3);
	return skinnedPosition;
}

void main()
{
	vec4 skinnedPosition = CalculateSkinnedPosition();
	gl_Position = projectionViewModel * skinnedPosition;
	textureCoordinate = vertexTextureCoordinate;
}
