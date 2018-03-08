#version 410

uniform mat4 projectionViewModel;
uniform samplerBuffer palette;

in vec3 vertexPosition;
in vec2 vertexTextureCoordinate;
in uvec4 jointIndices;
in vec3 jointWeights;

out vec2 textureCoordinate;

vec4 CalculateWeightedPosition(in mat4 jointTransform, in float jointWeight)
{
	return (jointTransform * vec4(vertexPosition, 1.0)) * jointWeight;
}

mat4 FetchJointTransform(in uint jointIndex)
{
	return mat4(
		texelFetch(palette, int(jointIndex) * 4),
		texelFetch(palette, int(jointIndex) * 4 + 1),
		texelFetch(palette, int(jointIndex) * 4 + 2),
		texelFetch(palette, int(jointIndex) * 4 + 3));
}

vec4 WeightedPositionForJoint(in uint jointIndex, in float jointWeight)
{
	mat4 jointTransform = FetchJointTransform(jointIndex);
	return CalculateWeightedPosition(jointTransform, jointWeight);
}

vec4 CalculateSkinnedPosition()
{
	vec4 skinnedPosition = vec4(0.0);
	skinnedPosition += WeightedPositionForJoint(jointIndices.x, jointWeights.x);
	skinnedPosition += WeightedPositionForJoint(jointIndices.y, jointWeights.y);
	skinnedPosition += WeightedPositionForJoint(jointIndices.z, jointWeights.z);
	skinnedPosition += WeightedPositionForJoint(jointIndices.w, 1.0 - (jointWeights.x + jointWeights.y + jointWeights.z));
	return skinnedPosition;
}

void main()
{
	vec4 skinnedPosition = CalculateSkinnedPosition();
	gl_Position = projectionViewModel * skinnedPosition;
	textureCoordinate = vertexTextureCoordinate;
}
