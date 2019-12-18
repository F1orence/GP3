#version 400

attribute vec3 pos;

layout (location = 2) in vec2 texCoord; // both work

varying vec2 TexCoord;

uniform mat4 MVP;

void main()
{
	TexCoord = texCoord;
	gl_Position = MVP * vec4(pos, 1.0);
}