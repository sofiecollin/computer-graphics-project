#version 150

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec2 texCoord;
in vec3 fragNormal;
in vec3	fragPos;
in vec3 sVec[4];
in vec3 camPos;

uniform sampler2D texConc;
uniform sampler2D texCarpet;
uniform vec3 lightSourcesColorArr[160];
uniform float radii[160];
uniform vec3 lightSourcesDirPosArr[160];
uniform bool isDirectional[160];
uniform Material material;
uniform bool useDiffTex;
uniform bool useSpecTex;


out vec4 outColor;

float kd = 0.30;
float ks = 0.9;
float ka = 0.2;
float alpha = 168.0;
float kl = 0.014;
float kq = 0.0007;


// Old
void main(void)
{
	vec3 vVec = normalize(-(fragPos - camPos));
	vec3 nVec = normalize(fragNormal);
	vec3 nsVec[4];
	vec3 hold = vec3 (0.0, 0.0, 0.0);
	
	if (useSpecTex == 1)
	{
		;
	}

	// Set diff/spec texts, or use standard values
	// if (useDiffTex == 1) 
	// {
	// 	vec3 diff = vec3(texture(material.diffuse, texCoord));
	// }
	// else 
	// {
	// 	vec3 diff = vec3(kd, kd, kd);
	// }
	// if (useSpecTex == 1) 
	// {
	// 	vec3 spec = vec3(texture(material.specular, texCoord));
	// }
	// else 
	// {
	// 	vec3 spec = vec3(ks, ks, ks);
	// }

	for (int i = 0; i < 4; i ++)
	{	
		if (!isDirectional[i])
		{
			// float dist = length(fragPos-lightSourcesDirPosArr[i]);
			// if (dist < radii[i])
			// {
			float dirx = lightSourcesDirPosArr[i].x;
			float diry = lightSourcesDirPosArr[i].y;
			float dirz = lightSourcesDirPosArr[i].z;
			vec3 lightPos = vec3(dirx, diry, dirz);
			vec3 ho = lightPos-fragPos;
			float l = length(ho);
			nsVec[i] = normalize(sVec[i]);
			//diffuse lighting
			vec3 hej = vec3(texture(material.diffuse, texCoord));
			
			vec3 diffuse = hej*lightSourcesColorArr[i]*max(0,dot(nVec, nsVec[i]));
	
			// diffuse without texture
			// vec3 diffuse = kd*lightSourcesColorArr[i]*max(0,dot(nVec, nsVec[i]));

			//Specular lighting
			vec3 specTex = vec3(texture(material.specular, texCoord));
			vec3 nrVec = 2 * nVec * dot(nVec, nsVec[i]) - nsVec[i];
			vec3 nvVec = normalize(vVec);
			vec3 spec = specTex * lightSourcesColorArr[i]*pow(max(0, dot(nrVec, nvVec)), alpha);
			// Spec wihtout tex
			// vec3 spec = ks * lightSourcesColorArr[i]*pow(max(0, dot(nrVec, nvVec)), alpha);

			float F = 1/(1+kl*l + kq*l*l);
			
			hold = F*(spec + diffuse) + hold;
		}
		else
		{
			nsVec[i] = normalize(sVec[i]);
			//diffuse lighting
			vec3 hej = vec3(texture(material.diffuse, texCoord));
			vec3 diffuse = hej*lightSourcesColorArr[i]*max(0,dot(nVec, nsVec[i]));
			// Diffuse without texture
			// vec3 diffuse = kd*lightSourcesColorArr[i]*max(0,dot(nVec, nsVec[i]));

			//Specular lighting
			vec3 specTex = vec3(texture(material.specular, texCoord));
			vec3 nrVec = 2 * nVec * dot(nVec, nsVec[i]) - nsVec[i];
			vec3 nvVec = normalize(vVec);
			vec3 spec = specTex * lightSourcesColorArr[i]*pow(max(0, dot(nrVec, nvVec)), alpha);
			// Spec without texture
			// vec3 spec = ks * lightSourcesColorArr[i]*pow(max(0, dot(nrVec, nvVec)), alpha);

			hold = spec + diffuse + hold;
		}
	}
	vec4 amb = ka*texture(texConc, texCoord);

	outColor = amb + vec4(hold, 1.0);
}
