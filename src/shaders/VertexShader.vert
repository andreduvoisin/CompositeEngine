#version 410

uniform mat4 projectionViewModel;
uniform samplerBuffer palette;

in vec3 vertexPosition;
in vec2 vertexTextureCoordinate;
in float jointWeights[4];
in int jointIndices[4];

out vec2 textureCoordinate;

mat4 TexturePaletteForIndex(in int index)
{
	return mat4(
		texelFetch(palette, index * 4),
		texelFetch(palette, index * 4 + 1),
		texelFetch(palette, index * 4 + 2),
		texelFetch(palette, index * 4 + 3));
}

void main()
{
	vec4 weightedPosition = vec4(0.0);
	
	mat4 texturePalette = TexturePaletteForIndex(jointIndices[0]);
	weightedPosition += ((texturePalette * vec4(vertexPosition, 1.0)) * jointWeights[0]);
	texturePalette = TexturePaletteForIndex(jointIndices[1]);
	weightedPosition += ((texturePalette * vec4(vertexPosition, 1.0)) * jointWeights[1]);
	texturePalette = TexturePaletteForIndex(jointIndices[2]);
	weightedPosition += ((texturePalette * vec4(vertexPosition, 1.0)) * jointWeights[2]);
	texturePalette = TexturePaletteForIndex(jointIndices[3]);
	weightedPosition += ((texturePalette * vec4(vertexPosition, 1.0)) * jointWeights[3]);
	
	gl_Position = projectionViewModel * weightedPosition;
	textureCoordinate = vertexTextureCoordinate;
}
