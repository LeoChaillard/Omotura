#pragma once

#include "../Core/Light.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

namespace Omotura
{
	std::string GetFileContents(const char* _pFileName);

	class Shader
	{
	public:
		GLuint m_ID;
		Shader() = default;
		Shader(const char* _pVertexFile, const char* _pFragmentFile);
		Shader(const char* _pVertexFile, const char* _pFragmentFile, const char* _pTesselControlFile, const char* _pTesselEvalFile);

		void Activate();
		void Delete();

		// Uniform setters
		void SetInt(const char* _pUniform, int _iValue);
		void SetFloat(const char* _pUniform, float _fValue);
		void SetFloat3(const char* _pUniform, float _fX, float _fY, float _fZ);
		void SetFloat4(const char* _pUniform, float _fX, float _fY, float _fZ, float _fW);
		void SetMatrixFloat4(const char* _pUniform, const glm::mat4& _matrix);

		// Lights setters
		void SetDirLight(const DirLight& _dirLight);
		void SetPointLight(const PointLight& _pointLight, int _iIndex);
		void SetSpotLight(const SpotLight& _spotLight, int _iIndex);

	private:
		void CompileLinkErrors(unsigned int _shader, const char* _pType);
	};
}