#version 400

varying vec2 TexCoord;

uniform sampler2D diffuse;

void main()
{	
	vec4 finalColour = texture2D(diffuse, TexCoord);
	
	if (finalColour.rgb == vec3(1,1,1))
	{
		finalColour = vec4(0,0,0,0);
	}
	else if (finalColour.rgb == vec3(1,0,1))
	{
		finalColour = vec4(0,0,0,0.4);
	}

	gl_FragColor = finalColour;
}
