#version 410

uniform sampler2D diffuseTexture;

in vec2 textureCoordinate;

out vec4 fragmentColor;

void main()
{
	fragmentColor = (bitCount(gl_SampleMaskIn[0]) < gl_NumSamples) ? vec4(1.0, 0.0, 0.0, 1.0) : texture(diffuseTexture, textureCoordinate);//vec4(0.0, 0.0, 0.0, 0.0);
}
