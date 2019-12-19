#version 400

attribute vec3 pos;

layout (location = 2) in vec2 texCoord; // both work

varying vec2 TexCoord;
uniform mat4 modelMat;
uniform mat4 MVP;


void main()
{
	TexCoord = texCoord;
	vec4 pPos = MVP * vec4(pos,1.0);
	//gl_Position = vec4(pPos.x,pPos.y,-pos.z,1);
	gl_Position = vec4(pos.x,pos.y,pos.z + 100,1.0);
}