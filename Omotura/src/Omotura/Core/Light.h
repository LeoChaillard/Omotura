#pragma once

#include <glm/glm.hpp>

namespace Omotura
{
	struct DirLight
	{
		glm::vec3 vDirection;

		glm::vec3 vAmbient;
		glm::vec3 vDiffuse;
		glm::vec3 vSpecular;
	};

	struct PointLight
	{
		glm::vec3 vPosition;

		float fConstant;
		float fLinear;
		float fQuadratic;

		glm::vec3 vAmbient;
		glm::vec3 vDiffuse;
		glm::vec3 vSpecular;
	};

	struct SpotLight
	{
		glm::vec3 vDirection;
		glm::vec3 vPosition;

		float fInnerCone;
		float fOuterCone;

		glm::vec3 vAmbient;
		glm::vec3 vDiffuse;
		glm::vec3 vSpecular;
	};
}