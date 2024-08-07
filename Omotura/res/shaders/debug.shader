#shader vertex
#version 420 core

layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{		
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1);
}

#shader fragment
#version 420 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
	FragColor = vec4(color, 1.0f);
}	