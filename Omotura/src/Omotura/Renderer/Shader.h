#pragma once

#include "../Asset/Asset.h"
#include "../Core/Light.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <cerrno>
#include <unordered_map>

namespace Omotura
{
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1,
		TESS_CONTROL = 2,
		TESS_EVALUATION = 3
	};

	struct ShaderSources
	{
		const char* pVertexShader;
		const char* pFragmentShader;
		const char* pTesselControlShader;
		const char* pTesselEvalShader;
	};

	class Shader : public Asset
	{
	private:
		std::string m_strFilePath;
		std::unordered_map<ShaderType, std::string> m_shaderSources;
		GLuint m_ID;

		using UniformHandle = UUID;
		std::unordered_map<UniformHandle, int> m_uniformCache;

	public:
		Shader();

		void Load(const char* _pShaderFilePath);
		void HotloadShaders();
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

		static AssetType GetStaticType() { return AssetType::SHADER; }
		virtual AssetType GetType() const { return GetStaticType(); }

	private:
		void ParseShader(const char* _pShaderCode);
		void CreateProgram();
		void CompileLinkErrors(unsigned int _shader, const char* _pType);
		int FindUniformLocation(const std::string& _strUniform);
	};
}