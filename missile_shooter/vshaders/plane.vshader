#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos; //fragment's position in world space to work w/ normal in world space
  //and lighting calculations

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	FragPos = vec3(model * vec4(aPos, 1.0f)); //multiply model w/ vector to put it in world
	  //space, then convert it to a vec3 to pass to an out FragPos
	Normal = normal * aNormal;
}