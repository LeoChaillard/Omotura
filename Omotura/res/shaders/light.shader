#shader vertex
#version 420 core

layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 camMatrix;

void main()
{
	gl_Position = camMatrix * modelMatrix * vec4(aPos, 1.0f);
}

#shader fragment
#version 420 core

out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
	FragColor = vec4(lightColor, 1.0f);
}