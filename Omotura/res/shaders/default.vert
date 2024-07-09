#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 crntPos;
out vec3 Normal;
out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

void main()
{
	crntPos = vec3(modelMatrix * vec4(aPos, 1.0f));			
	Normal = mat3(transpose(inverse(modelMatrix))) * aNormal; // !!! should be done on CPU !!!!!!
	texCoord = aTex;	
	gl_Position = camMatrix * vec4(crntPos, 1);
}