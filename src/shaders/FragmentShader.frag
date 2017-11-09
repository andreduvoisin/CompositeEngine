#version 410

uniform sampler2D ourTexture;

in vec2 texCoord;
in vec4 debugColor;

out vec4 frag_colour;

void main()
{
	if (debugColor.a != 0)
	{
		frag_colour = debugColor;
	}
	else
	{
		frag_colour = texture(ourTexture, texCoord);
	}
}
