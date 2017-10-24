#version 410

uniform mat4 mvp;
uniform samplerBuffer palette;

in vec4 vp;
in vec2 tp;

out vec2 texCoord;

void main() {
	gl_Position = mvp * vp;
	texCoord = tp;
}
