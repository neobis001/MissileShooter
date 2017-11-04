#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Material {
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};


uniform vec3 cameraPos; //in world space coords

uniform Material material;
uniform Light light;


const float ambientStrength = 0.1f;
const float specularStrength = 0.5f;

void main()
{
	//
	//ambient color calculations
	vec3 ambientColor = material.ambientColor * light.ambientColor;

	//
	//diffuse color calculations
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); //the normal points away from a triangle
	  //want lightDir to point away from triangle too for proper caluclations

	float diffInfluence = max(dot(norm, lightDir), 0.0f);
	vec3 diffuseColor = (diffInfluence * material.diffuseColor) * light.diffuseColor;

	//
	//specular color calculations
	vec3 cameraDir = normalize(cameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); //have the lightDir vector from above point towards the fragment
	  //from the GLSL spec formula, the resulting vector will be a "reflection" pointing away from the fragment
	float specInfluence = pow(max(dot(cameraDir, reflectDir), 0.0f), 32);
	vec3 specColor = (specInfluence * material.specularColor) * light.specularColor;

	vec3 resColor = ambientColor + diffuseColor + specColor;
    FragColor = vec4(resColor, 1.0f);
}
