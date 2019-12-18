#version 400

in vec2 TexCoord;
in vec3 Normals;
in vec3 VertPos;

uniform sampler2D diffuse;

uniform mat4 modelMat;

uniform vec3 lightDir;

uniform vec3 ambLight;

uniform float specValue;

uniform vec3 pLightDir;

uniform float pLightInt;

void main()
{	

	vec3 updatedNormal = normalize(mat3(transpose(inverse(modelMat))) * Normals);

	vec4 finalColour = texture2D(diffuse, TexCoord);

	// ambient

	//finalColour += vec4(ambLight, 1);
	//finalColour /= 2;

	finalColour += vec4((ambLight.x * 2) - 1, (ambLight.y * 2) - 1, (ambLight.z  * 2) - 1, 0); // 0.5,0.5,0.5 is white light 

	if (finalColour.x < 0)
	{
		finalColour.x = 0;
	}

	if (finalColour.y < 0)
	{
		finalColour.y = 0;
	}
	
	if (finalColour.z < 0)
	{
		finalColour.z = 0;
	}

	// diffuse
	float diffuseIntensity =  dot(updatedNormal,normalize(lightDir)); // how similar is the direction of surface normal to the direction of light

	if (diffuseIntensity > 1)
	{
		diffuseIntensity = 1;
	}

	finalColour.r *= (0.95 * diffuseIntensity);
	finalColour.g *= (0.95 * diffuseIntensity);
	finalColour.b *= (0.95 * diffuseIntensity);

	// specular
	vec3 viewDir = normalize(-VertPos);
	vec3 reflectionAngle = reflect(lightDir,  updatedNormal);

	float specularIntensity = pow(max(dot(viewDir, reflectionAngle), 0), 16); // how similar is the viewing angle to the direction of light

	finalColour += vec4(vec3(specularIntensity * specValue), 1);

	// point light
	if (pLightInt > 0)
	{ 
		float plIntensity =  dot(updatedNormal,normalize(pLightDir)); // how similar is the direction of surface normal to the direction of light

		if (plIntensity > 1)
		{
			plIntensity = 1;
		}

		finalColour.r += plIntensity * pLightInt;
		finalColour.g += plIntensity * pLightInt;
		finalColour.b += plIntensity * pLightInt;
		
		// specular

		reflectionAngle = reflect(pLightDir,  updatedNormal);

		float specularIntensity = pow(max(dot(viewDir, reflectionAngle), 0), 30); // how similar is the viewing angle to the direction of light

		finalColour += vec4(vec3(specularIntensity * specValue), 1);
	}

	gl_FragColor = finalColour;

}
