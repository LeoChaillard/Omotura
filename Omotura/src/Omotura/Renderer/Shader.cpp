#include "Shader.h"

#include "../Utils/Utils.hpp"
#include "../Common/Common.h"

#include <sstream>

namespace Omotura
{
	Shader::Shader()
		: m_strFilePath(),
		m_shaderSources(),
		m_ID()
	{
	}

	void Shader::Load(const char* _pShaderFilePath)
	{
		m_strFilePath = _pShaderFilePath;

		std::string strShaderCode = Utils::GetFileContents(_pShaderFilePath);

		if (!strShaderCode.empty())
		{
			ParseShader(strShaderCode.c_str());
			CreateProgram();
		}
	}

	void Shader::HotloadShaders()
	{
		if (m_strFilePath.empty())
		{
			return;
		}

		std::string strShaderCode = Utils::GetFileContents(m_strFilePath.c_str());

		if (!strShaderCode.empty())
		{
			ParseShader(strShaderCode.c_str());
			CreateProgram();
		}

	}
	
	void Shader::ParseShader(const char* _pShaderCode)
	{
		std::stringstream stream(_pShaderCode);
		ShaderType type = ShaderType::NONE;

		std::string strLine;
		while (getline(stream, strLine))
		{
			if (strLine.find("#shader") != std::string::npos)
			{
				if (strLine.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (strLine.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
				else if (strLine.find("tess control") != std::string::npos)
				{
					type = ShaderType::TESS_CONTROL;
				}
				else if (strLine.find("tess evaluation") != std::string::npos)
				{
					type = ShaderType::TESS_EVALUATION;
				}
			}
			else
			{
				m_shaderSources[type] += (strLine + "\n");
			}
		}
	}

	void Shader::CreateProgram()
	{
		m_ID = glCreateProgram();

		if (m_shaderSources.find(ShaderType::VERTEX) != m_shaderSources.end())
		{
			std::string strVertexSource = m_shaderSources[ShaderType::VERTEX];
			const char* pVertexsSource = strVertexSource.c_str();
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &pVertexsSource, NULL);
			glCompileShader(vertexShader);
			CompileLinkErrors(vertexShader, "VERTEX");
			glAttachShader(m_ID, vertexShader);
			glDeleteShader(vertexShader);
		}

		if (m_shaderSources.find(ShaderType::TESS_CONTROL) != m_shaderSources.end())
		{
			std::string strTessControlSource = m_shaderSources[ShaderType::TESS_CONTROL];
			const char* pTessControlSource = strTessControlSource.c_str();
			GLuint tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(tessControlShader, 1, &pTessControlSource, NULL);
			glCompileShader(tessControlShader);
			CompileLinkErrors(tessControlShader, "TESSELATION");
			glAttachShader(m_ID, tessControlShader);
			glDeleteShader(tessControlShader);

		}

		if (m_shaderSources.find(ShaderType::TESS_EVALUATION) != m_shaderSources.end())
		{
			std::string strTessEvalSource = m_shaderSources[ShaderType::TESS_EVALUATION];
			const char* pTessEvalSource = strTessEvalSource.c_str();
			GLuint tessEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(tessEvalShader, 1, &pTessEvalSource, NULL);
			glCompileShader(tessEvalShader);
			CompileLinkErrors(tessEvalShader, "TESSELATION");
			glAttachShader(m_ID, tessEvalShader);
			glDeleteShader(tessEvalShader);
		}

		if (m_shaderSources.find(ShaderType::FRAGMENT) != m_shaderSources.end())
		{
			std::string strFragmentSource = m_shaderSources[ShaderType::FRAGMENT];
			const char* pFragmentSource = strFragmentSource.c_str();
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &pFragmentSource, NULL);
			glCompileShader(fragmentShader);
			CompileLinkErrors(fragmentShader, "FRAGMENT");
			glAttachShader(m_ID, fragmentShader);
			glDeleteShader(fragmentShader);
		}

		glLinkProgram(m_ID);
		CompileLinkErrors(m_ID, "PROGRAM");
	}

	void Shader::CompileLinkErrors(unsigned int _shader, const char* _pType)
	{
		GLint hasCompiledLinked;
		char infoLog[1024];
		if (_pType != "PROGRAM")
		{
			glGetShaderiv(_shader, GL_COMPILE_STATUS, &hasCompiledLinked);
			if (hasCompiledLinked == GL_FALSE)
			{
				glGetShaderInfoLog(_shader, 1024, NULL, infoLog);
				std::cout << "SHADER_COMPILATION_ERROR for: " << _pType << "\n" << std::endl;
				std::cout << infoLog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(_shader, GL_LINK_STATUS, &hasCompiledLinked);
			if (hasCompiledLinked == GL_FALSE)
			{
				glGetShaderInfoLog(_shader, 1024, NULL, infoLog);
				std::cout << "SHADER_LINKING_ERROR for: " << _pType << "\n" << std::endl;
				std::cout << infoLog << std::endl;
			}
		}
	}

	void Shader::Activate()
	{
		glUseProgram(m_ID);
	}

	void Shader::Delete()
	{
		glDeleteProgram(m_ID);
	}

	int Shader::FindUniformLocation(const std::string& _strUniform)
	{
		UniformHandle handle = hashID(_strUniform.c_str());
		if (m_uniformCache.find(handle) == m_uniformCache.end())
		{
			int iAddr = glGetUniformLocation(m_ID, _strUniform.c_str());
			if (iAddr == -1)
			{
				return -1;
			}
			else
			{
				m_uniformCache[handle] = iAddr;
			}
			return iAddr;
		}
		return m_uniformCache[handle];
	}

	void Shader::SetInt(const char* _pUniform, int _iValue)
	{
		int iAddr = FindUniformLocation(std::string(_pUniform));
		if (iAddr != -1)
		{
			glUniform1i(iAddr, _iValue);
		}
	}

	void Shader::SetFloat(const char* _pUniform, float _fValue)
	{
		int iAddr = FindUniformLocation(std::string(_pUniform));
		if (iAddr != -1)
		{
			glUniform1f(iAddr, _fValue);
		}
	}

	void Shader::SetFloat3(const char* _pUniform, float _fX, float _fY, float _fZ)
	{
		int iAddr = FindUniformLocation(std::string(_pUniform));
		if (iAddr != -1)
		{
			glUniform3f(iAddr, _fX, _fY, _fZ);
		}
	}

	void Shader::SetFloat4(const char* _pUniform, float _fX, float _fY, float _fZ, float _fW)
	{
		int iAddr = FindUniformLocation(std::string(_pUniform));
		if (iAddr != -1)
		{
			glUniform4f(iAddr, _fX, _fY, _fZ, _fW);
		}
	}

	void Shader::SetMatrixFloat4(const char* _pUniform, const glm::mat4& _matrix)
	{
		int iAddr = FindUniformLocation(std::string(_pUniform));
		if (iAddr != -1)
		{
			glUniformMatrix4fv(iAddr, 1, GL_FALSE, glm::value_ptr(_matrix));
		}
	}

	void Shader::SetDirLight(const DirLight& _dirLight)
	{
		SetFloat3("dirLight.direction", _dirLight.vDirection.x, _dirLight.vDirection.y, _dirLight.vDirection.z);
		SetFloat3("dirLight.ambient", _dirLight.vAmbient.x, _dirLight.vAmbient.y, _dirLight.vAmbient.z);
		SetFloat3("dirLight.diffuse", _dirLight.vDiffuse.x, _dirLight.vDiffuse.y, _dirLight.vDiffuse.z);
		SetFloat3("dirLight.specular", _dirLight.vSpecular.x, _dirLight.vSpecular.y, _dirLight.vSpecular.z);
	}

	void Shader::SetPointLight(const PointLight& _pointLight, int _iIndex)
	{
		SetFloat3(std::string("pointLights[" + std::to_string(_iIndex) + "].lightPos").c_str(), _pointLight.vPosition.x, _pointLight.vPosition.y, _pointLight.vPosition.z);
		SetFloat(std::string("pointLights[" + std::to_string(_iIndex) + "].constant").c_str(), _pointLight.fConstant);
		SetFloat(std::string("pointLights[" + std::to_string(_iIndex) + "].linear").c_str(), _pointLight.fLinear);
		SetFloat(std::string("pointLights[" + std::to_string(_iIndex) + "].quadratic").c_str(), _pointLight.fQuadratic);
		SetFloat3(std::string("pointLights[" + std::to_string(_iIndex) + "].ambient").c_str(), _pointLight.vAmbient.x, _pointLight.vAmbient.y, _pointLight.vAmbient.z);
		SetFloat3(std::string("pointLights[" + std::to_string(_iIndex) + "].diffuse").c_str(), _pointLight.vDiffuse.x, _pointLight.vDiffuse.y, _pointLight.vDiffuse.z);
		SetFloat3(std::string("pointLights[" + std::to_string(_iIndex) + "].specular").c_str(), _pointLight.vSpecular.x, _pointLight.vSpecular.y, _pointLight.vSpecular.z);
	}

	void Shader::SetSpotLight(const SpotLight& _spotLight, int _iIndex)
	{
		SetFloat3("spotLight.lightPos", _spotLight.vPosition.x, _spotLight.vPosition.y, _spotLight.vPosition.z);
		SetFloat3("spotLight.direction", _spotLight.vDirection.x, _spotLight.vDirection.y, _spotLight.vDirection.z);
		SetFloat("spotLight.innerCone", _spotLight.fInnerCone);
		SetFloat("spotLight.outerCone", _spotLight.fOuterCone);
		SetFloat3("spotLight.ambient", _spotLight.vAmbient.x, _spotLight.vAmbient.y, _spotLight.vAmbient.z);
		SetFloat3("spotLight.diffuse", _spotLight.vDiffuse.x, _spotLight.vDiffuse.y, _spotLight.vDiffuse.z);
		SetFloat3("spotLight.specular", _spotLight.vSpecular.x, _spotLight.vSpecular.y, _spotLight.vSpecular.z);
	}
}