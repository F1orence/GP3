#version 400

attribute vec3 pos;

layout (location = 2) in vec2 texCoord; // both work
layout (location = 3) in vec3 normals; // both work

out vec2 TexCoord;
out vec3 Normals;
out vec3 VertPos;

uniform mat4 MVP;

void main()
{
	TexCoord = texCoord;
	Normals = normals;
	vec4 temp = MVP * vec4(pos, 1.0);
	VertPos = vec3(temp.x, temp.y, temp.z);
	gl_Position = temp;
}