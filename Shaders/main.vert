#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

uniform bool isDirectional[160];
uniform bool useHeightTex;
uniform int nrLights;
uniform float camx;
uniform float camy;
uniform float camz;
uniform vec3 lightSourcesDirPosArr[160];
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform sampler2D texHeight;

out vec2 texCoord;
out vec3 fragNormal;
out vec3 fragPos;
out vec3 sVec[5];
out vec3 camPos;
out mat4 mM;

void main(void)
{
	// Pos
	vec3 updatedPosition;
	mM = mdlMatrix;
	if(useHeightTex == true)
	{
		vec3 tex_height_vec = vec3(texture(texHeight, inTexCoord));
		float tex_height = length(tex_height_vec);
		vec3 normalNormals = normalize(inNormal);
		tex_height_vec = tex_height*normalNormals;
		updatedPosition = inPosition + tex_height_vec;
	}
	else
	{
		updatedPosition = inPosition;
	}
	
	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(updatedPosition, 1.0);
	fragPos = vec3(mdlMatrix * vec4(inPosition, 1.0));

	// Normals
	mat3 normalMatrix = mat3(mdlMatrix);
	fragNormal = normalize(normalMatrix * inNormal);

	// Tex coords
	texCoord = inTexCoord;

	// Camera
	camPos = vec3(camx, camy, camz);
	
	// sVec
	for (int i = 0; i < nrLights; i++)
	{
		vec4 transformedLightDir = vec4(lightSourcesDirPosArr[i], 1.0);
		if (isDirectional[i])
		{
			sVec[i] = normalize(-vec3(transformedLightDir));
		}
		else
		{
			vec3 lightViewDir = vec3(transformedLightDir) - fragPos;
			sVec[i] = normalize(lightViewDir);
		}
	}
	
}
