#version 150

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	float shininess;
};

in vec2 texCoord;
in vec3 fragNormal;
in vec3	fragPos;
in vec3 sVec[5];
in vec3 camPos;
in mat4 mM;

uniform sampler2D texMain;
uniform vec3 lightSourcesColorArr[160];
uniform mat4 mdlMatrix;
uniform float intensity[160];
uniform float kd;
uniform float ks;
uniform float ka;
uniform float alpha_spec;
uniform float alpha;
uniform float lightSourcekl[160];
uniform float lightSourcekq[160];
uniform vec3 lightSourcesDirPosArr[160];
uniform bool isDirectional[160];
uniform Material material;
uniform bool useDiffTex;
uniform bool useSpecTex;
uniform bool useNormTex;
uniform bool useAlpha;

out vec4 outColor;

float kl = 0.045;
float kq = 0.0075;

void main(void)
{
	vec3 vVec = normalize(-(fragPos - camPos));
	vec3 nVec = normalize(fragNormal);
	vec3 nsVec[5];
	vec3 hold = vec3 (0.0, 0.0, 0.0);
	vec3 diff, spec;	
	float alpha_color = 1.0;

	// Set diff/spec texts, or use standard values
	if (useDiffTex == true)	
	{
		float hej = kd * length(vec3(texture(material.diffuse, texCoord)));
		diff = vec3(hej, hej, hej);
	}
	else {diff = vec3(kd, kd, kd);}
	if (useSpecTex == true) 
	{
		float hej = ks * length(vec3(texture(material.specular, texCoord)));
		diff = vec3(hej, hej, hej);
	}
	else {spec = vec3(ks, ks, ks);}

	if (useAlpha == true){alpha_color = alpha;}
	if (useNormTex == true)
	{

		nVec = normalize(mat3(mM)*vec3(texture(material.diffuse, texCoord)));
	}

	for (int i = 0; i < 5; i ++)
	{	
		// If light is directional
		if (!isDirectional[i])
		{
			// Calc vectors
			float dirx = lightSourcesDirPosArr[i].x;
			float diry = lightSourcesDirPosArr[i].y;
			float dirz = lightSourcesDirPosArr[i].z;
			vec3 lightPos = vec3(dirx, diry, dirz);
			vec3 lvec = lightPos-fragPos;
			float l = length(lvec);
			nsVec[i] = normalize(sVec[i]);
			//diffuse lighting
			vec3 diffuse = diff*intensity[i]*lightSourcesColorArr[i]*max(0,dot(nVec, nsVec[i]));
			//Specular lighting
			vec3 nrVec = 2 * nVec * dot(nVec, nsVec[i]) - nsVec[i];
			vec3 nvVec = normalize(vVec);
			vec3 spec = spec*intensity[i] *lightSourcesColorArr[i]*pow(max(0, dot(nrVec, nvVec)), alpha_spec);
			
			float F = 1/(1+lightSourcekl[i]*l + lightSourcekq[i]*l*l);

			hold = F*(spec + diffuse) + hold;
		}
		// Else calc for directional light
		else
		{
			nsVec[i] = normalize(sVec[i]);
			//diffuse lighting
			vec3 hej = vec3(texture(material.diffuse, texCoord));
			vec3 diffuse = diff*intensity[i]*lightSourcesColorArr[i]*max(0,dot(nVec, nsVec[i]));

			//Specular lighting
			vec3 nrVec = 2 * nVec * dot(nVec, nsVec[i]) - nsVec[i];
			vec3 nvVec = normalize(vVec);
			vec3 spec = spec*intensity[i] * lightSourcesColorArr[i]*pow(max(0, dot(nrVec, nvVec)), alpha_spec);
			
			hold = spec + diffuse + hold;
		}
	}
	vec4 amb = ka*texture(texMain, texCoord);

	outColor = amb + vec4(hold, alpha_color);
}
