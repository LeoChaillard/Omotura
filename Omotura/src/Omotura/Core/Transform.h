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
		Vector3 m_vLocalPosition;

		Quaternion m_quaternion;

		Vector3 m_vWorldScale;
		Vector3 m_vLocalScale;

		// TODO : manage child and parents transforms
	public:
		Transform() = default;
		~Transform() = default;
	};
}