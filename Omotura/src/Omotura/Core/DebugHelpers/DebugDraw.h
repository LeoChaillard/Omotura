#pragma once

#include "../Transform.h"

#include "../../Utils/Math.hpp"
#include "../../Common/Common.h"
#include "../../Renderer/Shader.h"

#include <queue>
#include <map>

namespace Omotura
{
	enum class PrimitiveType
	{
		LINE,
		CROSS,
		SPHERE,
		CIRCLE,
		AXES,
		TRIANGLE,
		AABB,
		OBB,
		STRING,
		NONE
	};

	struct Primitive
	{
		Vector3 vColor;
		float fDuration;
		bool bDepthEnabled;
	};

	struct Line : public Primitive
	{
		Vector3 vFrom;
		Vector3 vTo;
		float fLineWidth;	
	};

	struct Cross : public Primitive
	{
		Vector3 vPosition;
		float fSize;
	};

	struct Sphere : public Primitive
	{
		Vector3 vCenterPosition;
		float fRadius;
	};

	struct Circle : public Primitive
	{
		Vector3 vCenterPosition;
		Vector3 vPlaneNormal;
		float fRadius;
	};

	struct Axes : public Primitive
	{
		Transform xfm;
		float fSize;
	};

	struct Triangle : public Primitive
	{
		Vector3 vVertex0;
		Vector3 vVertex1;
		Vector3 vVertex2;
		float fLineWidth;
		float fSize;
	};

	struct AABB : public Primitive
	{
		Vector3 vMinCoords;
		Vector3 vMaxCoords;
		float fLineWidth;
	};

	struct OBB : public Primitive
	{
		Transform centerTransform;
		Vector3 vScale;
		float fLineWidth;
	};

	struct String : public Primitive
	{
		Vector3 vPosition;
		const char* pText;
	};

	struct DrawCommand
	{
		PrimitiveType type;
		Shared<Primitive> pPrimitive;
	};

	class DebugDraw
	{
	private:
		static std::queue<DrawCommand> m_drawQueue;

		static Shader m_debugShader;

	public:
		// Adds a line segment to te debug drawing queue
		static void AddLine(const Vector3& _vFrom,
							const Vector3& _vTo,
							const Vector3& _vColor,
							float _fLineWidth = 1.0f,
							float _fDuration = 0.0f,
							bool _bDepthEnabled = true);

		// Adds an axis-aligned cross to the debug drawing queue
		static void AddCross(const Vector3& _vPosition,
			const Vector3& _vColor,
			float _fSize,
			float _fDuration = 0.0f,
			bool _bDepthEnabled = true);

		// Adds a wireframe sphere to the debug drawing queue
		static void AddSphere(const Vector3& _vCenterPosition,
			float _fRadius,
			const Vector3& _vColor,
			float _fDuration = 0.0f,
			bool _bDepthEnabled = true);

		// Adds a circle to the debug drawing queue
		static void AddCircle(const Vector3& _vCenterPosition,
			const Vector3& _vPlaneNormal,
			float _fRadius,
			const Vector3& _vColor,
			float _fDuration = 0.0f,
			bool _bDepthEnabled = true);

		// Adds a set of coordinate axes depicting the
		// position and orientation of the given
		// tranformation to the debug drawing queue
		static void AddAxes(const Transform& _xfm,
			const Vector3& _vColor,
			float _fSize,
			float _fDuration = 0.0f,
			bool _bDepthEnabled = true);

		// Adds a wireframe triangle to the debug drawing queue
		static void AddTriangle(const Vector3& _vertex0,
			const Vector3& _vertex1,
			const Vector3& _vertex2,
			const Vector3& _vColor,
			float _fLineWidth = 1.0f,
			float _fDuration = 0.0f,
			bool _bDepthEnabled = true);

		// Adds an axis-aligned bounding box to the debug drawing queue
		static void AddAAB(const Vector3& _vMinCoords,
			const Vector3& _vMaxCoords,
			const Vector3& _vColor,
			float _fLineWidth = 1.0f,
			float _fDuration = 0.0f,
			bool _bDepthEnabled = true);

		// Adds an oriented bounding box to the debug drawing queue
		static void AddOBB(const Transform _centerTransform,
			const Vector3& _vScale,
			const Vector3& _vColor,
			float _fLineWidth = 1.0f,
			float _fDuration = 0.0f,
			bool _bDepthEnabled = true);

		// Adds a text string to the debug drawing queue
		static void AddString(const Vector3& _vPosition,
			const char* _pText,
			const Vector3& _vColor,
			float _fDuration = 0.0f,
			bool _bDepthEnabled = true);

		static void DrawPrimitives(Shader& _shader);

	private:
		static void DrawLine(const DrawCommand& _command);
		static void DrawCross(const DrawCommand& _command);
		static void DrawSphere(const DrawCommand& _command);
		static void DrawCircle(const DrawCommand& _command);
		static void DrawAxes(const DrawCommand& _command);
		static void DrawTriangle(const DrawCommand& _command);
		static void DrawAABB(const DrawCommand& _command, bool _bFromOBB = false);
		static void DrawOBB(const DrawCommand& _command);
		static void DrawString(const DrawCommand& _command);
	};
}