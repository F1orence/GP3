#version 400

uniform vec3 objectColor;

varying vec2 TexCoord;

uniform sampler2D diffuse;

void main()
{	
	vec4 temp = texture2D(diffuse, TexCoord); //vec4(objectColor,1);
	vec4 final = vec4(1,1,1,2) - temp;
	gl_FragColor = final;

}
