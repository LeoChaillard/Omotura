#include "DebugDraw.h"

#include "../Game.h"

#include "../../Renderer/RendererCommon.h"

#include <glad/glad.h>

namespace Omotura
{
	std::queue<DrawCommand> DebugDraw::m_drawQueue;
	Shader DebugDraw::m_debugShader;

	/******************ADD QUEUE********************/
	void DebugDraw::AddLine(const Vector3& _vFrom,
		const Vector3& _vTo,
		const Vector3& _vColor,
		float _fLineWidth /*= 1.0f*/,
		float _fDuration /*= 0.0f*/,
		bool _bDepthEnabled /*= true*/)
	{
		Line line;
		line.vFrom = _vFrom;
		line.vTo = _vTo;
		line.vColor = _vColor;
		line.fLineWidth = _fLineWidth;
		line.fDuration = _fDuration;
		line.bDepthEnabled = _bDepthEnabled;

		DrawCommand command;
		command.type = PrimitiveType::LINE;
		command.pPrimitive = CreateShared<Line>(line);

		m_drawQueue.push(command);
	}

	void DebugDraw::AddCross(const Vector3& _vPosition,
		const Vector3& _vColor,
		float _fSize,
		float _fDuration /*= 0.0f*/,
		bool _bDepthEnabled /*= true*/)
	{
		Cross cross;
		cross.vPosition = _vPosition;
		cross.vColor = _vColor;
		cross.fSize= _fSize;
		cross.fDuration = _fDuration;
		cross.bDepthEnabled = _bDepthEnabled;

		DrawCommand command;
		command.type = PrimitiveType::CROSS;
		command.pPrimitive = CreateShared<Cross>(cross);

		m_drawQueue.push(command);
	}

	void DebugDraw::AddSphere(const Vector3& _vCenterPosition,
		float _fRadius,
		const Vector3& _vColor,
		float _fDuration /*= 0.0f*/,
		bool _bDepthEnabled /*= true*/)
	{
		Sphere sphere;
		sphere.vCenterPosition= _vCenterPosition;
		sphere.fRadius = _fRadius;
		sphere.vColor = _vColor;
		sphere.fDuration = _fDuration;
		sphere.bDepthEnabled = _bDepthEnabled;

		DrawCommand command;
		command.type = PrimitiveType::SPHERE;
		command.pPrimitive = CreateShared<Sphere>(sphere);

		m_drawQueue.push(command);
	}

	void DebugDraw::AddCircle(const Vector3& _vCenterPosition,
		const Vector3& _vPlaneNormal,
		float _fRadius,
		const Vector3& _vColor,
		float _fDuration /*= 0.0f*/,
		bool _bDepthEnabled /*= true*/)
	{
		Circle circle;
		circle.vCenterPosition = _vCenterPosition;
		circle.vPlaneNormal = _vPlaneNormal;
		circle.fRadius = _fRadius;
		circle.vColor = _vColor;
		circle.fDuration = _fDuration;
		circle.bDepthEnabled = _bDepthEnabled;

		DrawCommand command;
		command.type = PrimitiveType::CIRCLE;
		command.pPrimitive = CreateShared<Circle>(circle);

		m_drawQueue.push(command);
	}

	void DebugDraw::AddAxes(const Transform& _xfm,
		const Vector3& _vColor,
		float _fSize,
		float _fDuration /*= 0.0f*/,
		bool _bDepthEnabled /*= true*/)
	{
		Axes axes;
		axes.xfm = _xfm;
		axes.vColor = _vColor;
		axes.fSize = _fSize;
		axes.fDuration = _fDuration;
		axes.bDepthEnabled = _bDepthEnabled;

		DrawCommand command;
		command.type = PrimitiveType::AXES;
		command.pPrimitive = CreateShared<Axes>(axes);

		m_drawQueue.push(command);
	}

	void DebugDraw::AddTriangle(const Vector3& _vertex0,
		const Vector3& _vertex1,
		const Vector3& _vertex2,
		const Vector3& _vColor,
		float _fLineWidth /*= 1.0f*/,
		float _fDuration /*= 0.0f*/,
		bool _bDepthEnabled /*= true*/)
	{
		Triangle triangle;
		triangle.vVertex0 = _vertex0;
		triangle.vVertex1 = _vertex1;
		triangle.vVertex2 = _vertex2;
		triangle.vColor = _vColor;
		triangle.fLineWidth = _fLineWidth;
		triangle.fDuration = _fDuration;
		triangle.bDepthEnabled = _bDepthEnabled;

		DrawCommand command;
		command.type = PrimitiveType::TRIANGLE;
		command.pPrimitive = CreateShared<Triangle>(triangle);

		m_drawQueue.push(command);
	}

	void DebugDraw::AddAAB(const Vector3& _vMinCoords,
		const Vector3& _vMaxCoords,
		const Vector3& _vColor,
		float _fLineWidth /*= 1.0f*/,
		float _fDuration /*= 0.0f*/,
		bool _bDepthEnabled /*= true*/)
	{
		AABB aabb;
		aabb.vMinCoords = _vMinCoords;
		aabb.vMaxCoords = _vMaxCoords;
		aabb.vColor = _vColor;
		aabb.fLineWidth = _fLineWidth;
		aabb.fDuration = _fDuration;
		aabb.bDepthEnabled = _bDepthEnabled;

		DrawCommand command;
		command.type = PrimitiveType::AABB;
		command.pPrimitive = CreateShared<AABB>(aabb);

		m_drawQueue.push(command);
	}

	void DebugDraw::AddOBB(const Transform _centerTransform,
		const Vector3& _vScale,
		const Vector3& _vColor,
		float _fLineWidth /*= 1.0f*/,
		float _fDuration /*= 0.0f*/,
		bool _bDepthEnabled /*= true*/)
	{
		OBB obb;
		obb.centerTransform = _centerTransform;
		obb.vScale= _vScale;
		obb.vColor = _vColor;
		obb.fLineWidth = _fLineWidth;
		obb.fDuration = _fDuration;
		obb.bDepthEnabled = _bDepthEnabled;

		DrawCommand command;
		command.type = PrimitiveType::OBB;
		command.pPrimitive = CreateShared<OBB>(obb);

		m_drawQueue.push(command);
	}

	void DebugDraw::AddString(const Vector3& _vPosition,
		const char* _pText,
		const Vector3& _vColor,
		float _fDuration /*= 0.0f*/,
		bool _bDepthEnabled /*= true*/)
	{
		String string;
		string.vPosition = _vPosition;
		string.pText = _pText;
		string.vColor = _vColor;
		string.fDuration = _fDuration;
		string.bDepthEnabled = _bDepthEnabled;

		DrawCommand command;
		command.type = PrimitiveType::STRING;
		command.pPrimitive = CreateShared<String>(string);

		m_drawQueue.push(command);
	}

	/******************DRAW PRIMITIVES********************/
	void DebugDraw::DrawPrimitives(Shader& _shader)
	{
		m_debugShader = _shader;
		while (!m_drawQueue.empty())
		{
			const DrawCommand& command = m_drawQueue.front();
			switch (command.type)
			{
				case PrimitiveType::LINE: DrawLine(command); break;
				case PrimitiveType::CROSS: DrawCross(command); break;
				case PrimitiveType::SPHERE: DrawSphere(command); break;
				case PrimitiveType::CIRCLE: DrawCircle(command); break;
				case PrimitiveType::AXES: DrawAxes(command); break;
				case PrimitiveType::TRIANGLE: DrawTriangle(command); break;
				case PrimitiveType::AABB: DrawAABB(command); break;
				case PrimitiveType::OBB: DrawOBB(command); break;
				case PrimitiveType::STRING: DrawString(command); break;
			}
			m_drawQueue.pop();
		}
	}

	void DebugDraw::DrawLine(const DrawCommand& _command)
	{
		Shared<Line> pLine = std::static_pointer_cast<Line>(_command.pPrimitive);
		std::vector<glm::vec3> vPoints;
		vPoints.push_back(pLine->vFrom.ToGLM());
		vPoints.push_back(pLine->vTo.ToGLM());

		// Create and bind buffers
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vPoints[0]) * vPoints.size(), vPoints.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Setup shader
		m_debugShader.Activate();
		m_debugShader.SetFloat3("color", pLine->vColor.x, pLine->vColor.y, pLine->vColor.z);
		m_debugShader.SetMatrixFloat4("modelMatrix", glm::mat4(1.0f));
		m_debugShader.SetMatrixFloat4("viewMatrix", Game::GetInstance()->GetPlayerCamera()->GetViewMatrix());
		m_debugShader.SetMatrixFloat4("projectionMatrix", Game::GetInstance()->GetPlayerCamera()->GetPerspectiveMatrix());

		// Draw
		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 0, 2);

		// Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugDraw::DrawCross(const DrawCommand& _command)
	{

	}

	void DebugDraw::DrawSphere(const DrawCommand& _command)
	{
		Shared<Sphere> pSphere = std::static_pointer_cast<Sphere>(_command.pPrimitive);
		glm::vec3 vNormal = glm::vec3(1.0f, 0.0f, 0.0f);

		// Drawing a circle along the 3 world axis
		for (int i = 0; i < 3; i++)
		{
			Circle circle;
			circle.vCenterPosition = pSphere->vCenterPosition;
			circle.vPlaneNormal = Vector3(vNormal.x, vNormal.y, vNormal.z);
			circle.fRadius = pSphere->fRadius;;
			circle.vColor = pSphere->vColor;
			circle.fDuration = pSphere->fDuration;
			circle.bDepthEnabled = pSphere->bDepthEnabled;

			DrawCommand command;
			command.type = PrimitiveType::CIRCLE;
			command.pPrimitive = CreateShared<Circle>(circle);
			DrawCircle(command);

			vNormal = vNormal == glm::vec3(1.0f, 0.0f, 0.0f) ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(0.0f, 0.0f, 1.0f);
		}
	}


	void DebugDraw::DrawCircle(const DrawCommand& _command)
	{
		Shared<Circle> pCircle = std::static_pointer_cast<Circle>(_command.pPrimitive);
		std::vector<glm::vec3> vVertices;
		
		// Compute two perpendicular vectors to the plane normal vector
		glm::vec3 R = glm::vec3(1.0f, 0.0f, 0.0f);
		R = pCircle->vPlaneNormal.ToGLM() == R ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f); // choose another vector if R equals the plane normal vector
		glm::vec3 A = glm::normalize(glm::cross(pCircle->vPlaneNormal.ToGLM(), R));
		glm::vec3 B = glm::normalize(glm::cross(pCircle->vPlaneNormal.ToGLM(), A));
		
		// Compute the vertices' position on the circle		
		int iNumOfSides = 360;
		int iVertices = iNumOfSides * 2.0f;
		for (int i = 1; i < iVertices; i++) 
		{
			glm::vec3 vResult = (A * pCircle->fRadius * cos(i * 2.0f * glm::pi<float>() / iNumOfSides)) 
							  + (B * (pCircle->fRadius * sin(i * 2.0f * glm::pi<float>() / iNumOfSides)));
			vVertices.push_back(pCircle->vCenterPosition.ToGLM() + vResult);
		}

		// Create and bind buffers
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices[0]) * vVertices.size(), vVertices.data(), GL_STATIC_DRAW);

		// Set vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Setup shader
		m_debugShader.Activate();
		m_debugShader.SetFloat3("color", pCircle->vColor.x, pCircle->vColor.y, pCircle->vColor.z);
		m_debugShader.SetMatrixFloat4("modelMatrix", glm::mat4(1.0f));
		m_debugShader.SetMatrixFloat4("viewMatrix", Game::GetInstance()->GetPlayerCamera()->GetViewMatrix());
		m_debugShader.SetMatrixFloat4("projectionMatrix", Game::GetInstance()->GetPlayerCamera()->GetPerspectiveMatrix());

		// Draw
		glLineWidth(1.0f);
		glDrawArrays(GL_LINE_STRIP, 0, vVertices.size());

		// Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugDraw::DrawAxes(const DrawCommand& _command)
	{
		Shared<Axes> pAxes = std::static_pointer_cast<Axes>(_command.pPrimitive);
		std::vector<glm::vec3> vVertices;

		glm::vec3 vCenter(0.0f);
		glm::vec3 vXAxis = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 vYAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 vZAxis = glm::vec3(0.0f, 0.0f, 1.0f);

		// Axis lines
		vVertices.push_back(vCenter);
		vVertices.push_back(vXAxis);

		vVertices.push_back(vCenter);
		vVertices.push_back(vYAxis);

		vVertices.push_back(vCenter);
		vVertices.push_back(vZAxis);

		// Model Matrix
		glm::mat4 mTranslation = glm::translate(pAxes->xfm.m_vWorldPosition.ToGLM());
		glm::quat qRot = pAxes->xfm.m_quaternion.ToGLM();
		glm::mat4 mRot = glm::mat4_cast(qRot);
		glm::mat4 mScale = glm::mat4(pAxes->fSize);
		glm::mat4 modelMatrix = mTranslation * mRot * mScale;

		// Create and bind buffers
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices[0]) * vVertices.size(), vVertices.data(), GL_STATIC_DRAW);

		// Set vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Setup shader
		m_debugShader.Activate();
		m_debugShader.SetFloat3("color", pAxes->vColor.x, pAxes->vColor.y, pAxes->vColor.z);
		m_debugShader.SetMatrixFloat4("modelMatrix", modelMatrix);
		m_debugShader.SetMatrixFloat4("viewMatrix", Game::GetInstance()->GetPlayerCamera()->GetViewMatrix());
		m_debugShader.SetMatrixFloat4("projectionMatrix", Game::GetInstance()->GetPlayerCamera()->GetPerspectiveMatrix());

		// Draw
		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 0, vVertices.size());

		// Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugDraw::DrawTriangle(const DrawCommand& _command)
	{
		Shared<Triangle> pTriangle = std::static_pointer_cast<Triangle>(_command.pPrimitive);
		std::vector<glm::vec3> vVertices;
		vVertices.push_back(pTriangle->vVertex0.ToGLM());
		vVertices.push_back(pTriangle->vVertex1.ToGLM());
		vVertices.push_back(pTriangle->vVertex2.ToGLM());

		// Create and bind buffers
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices[0]) * vVertices.size(), vVertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Setup shader
		m_debugShader.Activate();
		m_debugShader.SetFloat3("color", pTriangle->vColor.x, pTriangle->vColor.y, pTriangle->vColor.z);
		m_debugShader.SetMatrixFloat4("modelMatrix", glm::mat4(1.0f));
		m_debugShader.SetMatrixFloat4("viewMatrix", Game::GetInstance()->GetPlayerCamera()->GetViewMatrix());
		m_debugShader.SetMatrixFloat4("projectionMatrix", Game::GetInstance()->GetPlayerCamera()->GetPerspectiveMatrix());

		// Draw
		glLineWidth(1.0f);
		glDrawArrays(GL_LINE_LOOP, 0, vVertices.size());

		// Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugDraw::DrawAABB(const DrawCommand& _command, bool _bFromOBB /*= false*/)
	{
		Shared<AABB> pAABB = std::static_pointer_cast<AABB>(_command.pPrimitive);
		std::vector<glm::vec3> vVertices;

		// Vector lengths
		glm::vec3 vXLength = glm::vec3(1.0f, 0.0f, 0.0f) * (pAABB->vMaxCoords.ToGLM().x - pAABB->vMinCoords.ToGLM().x);
		glm::vec3 vYLength = glm::vec3(0.0f, 1.0f, 0.0f) * (pAABB->vMaxCoords.ToGLM().y - pAABB->vMinCoords.ToGLM().y);
		glm::vec3 vZLength = glm::vec3(0.0f, 0.0f, 1.0f) * (pAABB->vMaxCoords.ToGLM().z - pAABB->vMinCoords.ToGLM().z);

		// First Loop
		vVertices.push_back(pAABB->vMinCoords.ToGLM());
		vVertices.push_back(pAABB->vMinCoords.ToGLM() + vYLength);
		vVertices.push_back(pAABB->vMinCoords.ToGLM() + vYLength + vZLength);
		vVertices.push_back(pAABB->vMinCoords.ToGLM() + vZLength);

		// Second Loop
		vVertices.push_back(pAABB->vMaxCoords.ToGLM());
		vVertices.push_back(pAABB->vMaxCoords.ToGLM() - vYLength);
		vVertices.push_back(pAABB->vMaxCoords.ToGLM() - vYLength - vZLength);
		vVertices.push_back(pAABB->vMaxCoords.ToGLM() - vZLength);

		// Connect Loops
		vVertices.push_back(pAABB->vMinCoords.ToGLM());
		vVertices.push_back(pAABB->vMinCoords.ToGLM() + vXLength);

		vVertices.push_back(pAABB->vMinCoords.ToGLM() + vYLength);
		vVertices.push_back(pAABB->vMinCoords.ToGLM() + vYLength + vXLength);

		vVertices.push_back(pAABB->vMinCoords.ToGLM() + vYLength + vZLength);
		vVertices.push_back(pAABB->vMinCoords.ToGLM() + vYLength + vZLength + vXLength);

		vVertices.push_back(pAABB->vMinCoords.ToGLM() + vZLength);
		vVertices.push_back(pAABB->vMinCoords.ToGLM() + vZLength + vXLength);

		// Create and bind buffers
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices[0]) * vVertices.size(), vVertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Setup shader
		m_debugShader.Activate();
		m_debugShader.SetFloat3("color", pAABB->vColor.x, pAABB->vColor.y, pAABB->vColor.z);
		m_debugShader.SetMatrixFloat4("viewMatrix", Game::GetInstance()->GetPlayerCamera()->GetViewMatrix());
		m_debugShader.SetMatrixFloat4("projectionMatrix", Game::GetInstance()->GetPlayerCamera()->GetPerspectiveMatrix());

		if (!_bFromOBB)
		{
			m_debugShader.SetMatrixFloat4("modelMatrix", glm::mat4(1.0f));
		}

		// Draw
		glLineWidth(1.0f);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		glDrawArrays(GL_LINE_LOOP, 4, 4);
		glDrawArrays(GL_LINES, 8, 8);

		// Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void DebugDraw::DrawOBB(const DrawCommand& _command)
	{
		Shared<OBB> pOBB = std::static_pointer_cast<OBB>(_command.pPrimitive);
		std::vector<glm::vec3> vVertices;

		// Model Matrix
		glm::mat4 mTranslation = glm::translate(pOBB->centerTransform.m_vWorldPosition.ToGLM());
		glm::quat qRot = pOBB->centerTransform.m_quaternion.ToGLM();
		glm::mat4 mRot = glm::mat4_cast(qRot);
		glm::mat4 mScale = glm::scale(pOBB->vScale.ToGLM());
		glm::mat4 modelMatrix = mTranslation * mRot * mScale;

		// 1x1 bounding box coordinates
		glm::vec3 vMinCoords = glm::vec3(-0.5f, -0.5f, -0.5f);
		glm::vec3 vMaxCoord = glm::vec3(0.5f, 0.5f, 0.5f);

		// Making an AABB and transforms will be applied in the shader
		AABB aabb;
		aabb.vMinCoords = Vector3(vMinCoords.x, vMinCoords.y, vMinCoords.z);
		aabb.vMaxCoords = Vector3(vMaxCoord.x, vMaxCoord.y, vMaxCoord.z);
		aabb.vColor = pOBB->vColor;
		aabb.fLineWidth = pOBB->fLineWidth;
		aabb.fDuration = pOBB->fDuration;
		aabb.bDepthEnabled = pOBB->fDuration;

		DrawCommand command;
		command.type = PrimitiveType::OBB;
		command.pPrimitive = CreateShared<AABB>(aabb);

		// Setup shader
		m_debugShader.Activate();
		m_debugShader.SetMatrixFloat4("modelMatrix", modelMatrix);

		DrawAABB(command, true);
	}

	void DebugDraw::DrawString(const DrawCommand& _command)
	{

	}
}