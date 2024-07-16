#pragma once

#include "../Renderer/Shader.h"
#include "../Common/Common.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Omotura
{
	class Camera
	{
	private:
		Transform m_transform;
		int m_iViewportWidth;
		int m_iViewportHeight;

	public:
		Camera(int _iWidth, int _iHeight, glm::vec3 _vPosition);

		void Follow(const Transform& _transform);

		glm::mat4 GetOrientation();
		glm::mat4 GetPerspectiveMatrix();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetInverseViewMatrix();

		const Transform& GetTransform() const;
	};
}
