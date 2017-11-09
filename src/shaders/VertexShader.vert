#version 410


/*
uniform mat4 mvp;
uniform samplerBuffer palette;

in vec4 vp;
in vec2 tp;

out vec2 texCoord;

void main()
{
	gl_Position = mvp * vp;
	texCoord = tp;
}
*/


uniform mat4 mvp;
uniform samplerBuffer palette;

in vec3 vp;
in vec2 tp;
in float jointWeights[4];
in int jointIndices[4];

out vec2 texCoord;
out vec4 debugColor;

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
	vec4 position = vec4(0.0);
	
	mat4 texturePalette = TexturePaletteForIndex(jointIndices[0]);
	position += ((texturePalette * vec4(vp, 1.0)) * jointWeights[0]);
	texturePalette = TexturePaletteForIndex(jointIndices[1]);
	position += ((texturePalette * vec4(vp, 1.0)) * jointWeights[1]);
	texturePalette = TexturePaletteForIndex(jointIndices[2]);
	position += ((texturePalette * vec4(vp, 1.0)) * jointWeights[2]);
	texturePalette = TexturePaletteForIndex(jointIndices[3]);
	position += ((texturePalette * vec4(vp, 1.0)) * jointWeights[3]);
	
	gl_Position = mvp * position;

	if (vp.x == 0 && vp.z == 50)
	{
		debugColor = vec4(jointWeights[0], jointWeights[0], jointWeights[0], 1.0);
		//debugColor = vec4(1.0, 1.0, 1.0, 1.0);
		gl_Position = mvp * vec4(vp, 1.0);
	}
	else if (vp.x == 50 && vp.z == 50)
	{
		debugColor = vec4(jointWeights[1], jointWeights[1], jointWeights[1], 1.0);
		//debugColor = vec4(1.0, 1.0, 1.0, 1.0);
		gl_Position = mvp * vec4(vp, 1.0);
	}
	else if (vp.x == 50 && vp.z == 100)
	{
		debugColor = vec4(jointWeights[2], jointWeights[2], jointWeights[2], 1.0);
		//debugColor = vec4(1.0, 1.0, 1.0, 1.0);
		gl_Position = mvp * vec4(vp, 1.0);
	}
	else if (vp.x == 0 && vp.z == 100)
	{
		debugColor = vec4(jointWeights[3], jointWeights[3], jointWeights[3], 1.0);
		//debugColor = vec4(1.0, 1.0, 1.0, 1.0);
		gl_Position = mvp * vec4(vp, 1.0);
	}
	else
	{
		debugColor = vec4(0.0, 0.0, 0.0, 0.0);
		texCoord = tp;
	}
}
