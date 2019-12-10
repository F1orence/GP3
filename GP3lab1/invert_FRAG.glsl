#version 400

uniform vec3 objectColor;

varying vec2 TexCoord;

uniform sampler2D diffuse;

void main()
{	
	gl_FragColor = vec4(1,0,1,1); //texture2D(diffuse, TexCoord); //vec4(objectColor,1);
}