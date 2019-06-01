#version 410

uniform sampler2D diffuseTexture;

in vec2 textureCoordinate;

out vec4 fragmentColor;

void main()
{
	// This is just a hacky way to render a wireframe given that MSAA is enabled.
	// Inspiration: https://twitter.com/iquilezles/status/1132034905813049345
	//
	// numSamplesWithinPolygon gives the number of MSAA samples that are taken
	// within the polygon. On the polygon edge, this will be less than gl_NumSamples,
	// as some of the samples will take place outside the polygon (hence anti aliasing).
	// On the polygon interior, this not be less than gl_NumSamples, as all of
	// the samples are taken within the polygon boundaries.
	int numSamplesWithinPolygon = bitCount(gl_SampleMaskIn[0]);
	bool isSamplingPolygonBoundary = numSamplesWithinPolygon < gl_NumSamples;
	if (isSamplingPolygonBoundary)
	{
		fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else
	{
		fragmentColor = texture(diffuseTexture, textureCoordinate);
	}
}
