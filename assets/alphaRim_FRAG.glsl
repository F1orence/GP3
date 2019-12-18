#version 400

in vec2 TexCoord;
in vec3 Normals;
in vec3 VertPos;

uniform sampler2D diffuse;

uniform mat4 modelMat;

uniform vec3 lightDir;

uniform vec3 ambLight;

uniform float specValue;

void main()
{	

	vec3 updatedNormal = mat3(transpose(inverse(modelMat))) * Normals;

	vec4 finalColour = texture2D(diffuse, TexCoord);

	// specular
	vec3 viewDir = normalize(-VertPos);
	float alpha = 1 - dot(updatedNormal, viewDir);

	gl_FragColor = vec4(0.8,0.8,0.8,0.8); //finalColour.r, finalColour.g, finalColour.b, alpha);

}
