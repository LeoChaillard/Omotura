#pragma once

#include "../Utils/Math.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Omotura
{
	class Transform
	{
	public:
		Vector3 m_vWorldPosition;
		glm::vec3 m_vLocalPosition = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::quat m_quaternion = glm::quat(1, 0, 0, 0);

		Vector3 m_vWorldScale;
		glm::vec3 m_vLocalScale = glm::vec3(1.0f, 1.0f, 1.0f);

		// TODO : manage child and parents transforms
	public:
		Transform() = default;
		~Transform() = default;
	};
}