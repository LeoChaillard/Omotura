#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aBoneWeights;

out vec3 crntPos;
out vec3 Normal;
out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

uniform mat4 matrixPalette[100];

void main()
{
	mat4 boneTransform = matrixPalette[aBoneIDs[0]] * aBoneWeights[0];
	boneTransform += matrixPalette[aBoneIDs[1]] * aBoneWeights[1];
	boneTransform += matrixPalette[aBoneIDs[2]] * aBoneWeights[2];
	boneTransform += matrixPalette[aBoneIDs[3]] * aBoneWeights[3];

	//if (aBoneIDs[0] < 87 && aBoneIDs[1] < 87 && aBoneIDs[2] < 87 && aBoneIDs[3] < 87)
	//{
			crntPos = vec3(modelMatrix * boneTransform * vec4(aPos, 1.0f));			
	//}
	//else
	//{
	//	crntPos = vec3(modelMatrix * vec4(aPos, 1.0f));
	//}
	Normal = mat3(transpose(inverse(modelMatrix))) * aNormal; // !!! should be done on CPU !!!!!!
	texCoord = aTex;	

	gl_Position = camMatrix * vec4(crntPos, 1);
}