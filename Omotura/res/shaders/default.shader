#shader vertex
#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 crntPos;
out vec3 Normal;
out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	crntPos = vec3(modelMatrix * vec4(aPos, 1.0f));			
	Normal = mat3(transpose(inverse(modelMatrix))) * aNormal; // !!! should be done on CPU !!!!!!
	texCoord = aTex;	
	gl_Position = projectionMatrix * viewMatrix * vec4(crntPos, 1);
}

#shader fragment
#version 420 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec2 texCoord;

struct Material
{
	sampler2D diffuse0;
	sampler2D normal0;
	sampler2D specular0;
	float shininess;
};
uniform Material material;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PLight
{
	vec3 lightPos;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NR_POINT_LIGHTS 1
uniform PLight pointLights[NR_POINT_LIGHTS];

struct SPLight
{
	vec3 lightPos;
	vec3 direction;

	float innerCone;
	float outerCone;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SPLight spotLight;

uniform vec3 camPos;

vec3 PointLight(PLight _light, vec3 _normal, vec3 _fragPos, vec3 _viewDir)
{	
	vec3 lightVec = _light.lightPos - _fragPos;

	// Intensity of light with respect to distance
	float dist = length(lightVec);
	float inten = 1.0f / (_light.quadratic * dist * dist + _light.linear * dist + _light.constant);

	// Ambient lighting
	vec3 ambient = _light.ambient * vec3(texture(material.diffuse0, texCoord));

	// Diffuse lighting
	vec3 lightDirection = normalize(lightVec);
	vec3 diffuse = _light.diffuse * max(dot(_normal, lightDirection), 0.0f) * vec3(texture(material.diffuse0, texCoord));

	// Specular lighting
	vec3 specular = vec3(0.0f, 0.0f, 0.0f);
	if (diffuse != vec3(0.0f))
	{
		vec3 normalMap = normalize(texture(material.normal0, texCoord).rgb * 2.0f - 1.0f);
		//vec3 reflectionDirection = reflect(-lightDirection, normalMap);
		vec3 halfway = normalize(_viewDir + lightDirection);
		float specAmount = pow(max(dot(normalMap, halfway), 0.0f), material.shininess);
		specular = _light.specular * specAmount * vec3(texture(material.specular0, texCoord).r);
	}

	return ((ambient * inten) + (diffuse * inten) + (specular * inten));
}

vec3 DirecLight(DirLight _light, vec3 _normal, vec3 _viewDir)
{
	// Ambient lighting
	vec3 ambient = _light.ambient * vec3(texture(material.diffuse0, texCoord));

	// Diffuse lighting
	vec3 lightDirection = normalize(_light.direction);
	vec3 diffuse = _light.diffuse * max(dot(_normal, lightDirection), 0.0f) * vec3(texture(material.diffuse0, texCoord));

	// Specular lighting
	vec3 specular = vec3(0.0f);
	if (diffuse != vec3(0.0f))
	{
		vec3 normalMap = normalize(texture(material.normal0, texCoord).rgb * 2.0f - 1.0f);
		//vec3 reflectionDirection = reflect(-lightDirection, normalMap);
		vec3 halfway = normalize(_viewDir + lightDirection);
		float specAmount = pow(max(dot(normalMap, halfway), 0.0f), material.shininess);
		specular = _light.specular * specAmount * vec3(texture(material.specular0, texCoord).r);
	}

	return (ambient + diffuse + specular);
}

vec3 SpotLight(SPLight _light, vec3 _normal, vec3 _fragPos, vec3 _viewDir)
{
	// Ambient lighting
	vec3 ambient = _light.ambient * vec3(texture(material.diffuse0, texCoord));

	// Diffuse lighting
	vec3 lightDirection = normalize(_light.lightPos - _fragPos);
	vec3 diffuse = _light.diffuse * max(dot(_normal, lightDirection), 0.0f) * vec3(texture(material.diffuse0, texCoord));

	// Specular lighting
	vec3 specular = vec3(0.0f, 0.0f, 0.0f);
	if (diffuse != vec3(0.0f))
	{
		vec3 normalMap = normalize(texture(material.normal0, texCoord).rgb * 2.0f - 1.0f);
		//vec3 reflectionDirection = reflect(-lightDirection, normalMap);
		vec3 halfway = normalize(_viewDir + lightDirection);
		float specAmount = pow(max(dot(normalMap, halfway), 0.0f), material.shininess);
		specular = _light.specular * specAmount * vec3(texture(material.specular0, texCoord).r);
	}

	// Calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - 0.90f) / (0.95f - 0.90f), 0.0f, 1.0f);

	return ((ambient * inten) + (diffuse * inten) + (specular * inten));
}

float LinearizeDepth(float _depth)
{
	float near = 0.1f;
	float far = 100.0f;
	float z = _depth * 2.0f - 1.0f; // back to NDC
	return (2.0f * near * far) / (far + near - z * (far - near));
}

void main()
{
	// Color that will be outputed
	vec3 fragOutput = vec3(0.0f);
	
	// Properties
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(camPos - crntPos);

	// Directional light
	fragOutput = DirecLight(dirLight, normal, viewDir);

	// Point lights
	//for (int i = 0; i < NR_POINT_LIGHTS; i++)
	//{
	//	fragOutput += PointLight(pointLights[i], normal, crntPos, viewDir);
	//}

	// Spot/flash lights
	fragOutput += SpotLight(spotLight, normal, crntPos, viewDir);

	// Final color
	FragColor = vec4(fragOutput, 1.0f);
	//FragColor = texture(material.diffuse0, texCoord);
}