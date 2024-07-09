#include "ShaderClass.h"

namespace Omotura
{
	std::string GetFileContents(const char* _pFileName)
	{
		std::ifstream in(_pFileName, std::ios::binary);
		if (in)
		{
			std::string strContents;
			in.seekg(0, std::ios::end);
			strContents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&strContents[0], strContents.size());
			in.close();
			return strContents;
		}
		throw(errno);
	}

	Shader::Shader(const char* _pVertexFile, const char* _pFragmentFile)
	{
		std::string strVertexCode = GetFileContents(_pVertexFile);
		std::string strFragmentCode = GetFileContents(_pFragmentFile);

		const char* vertexSource = strVertexCode.c_str();
		const char* fragmentSource = strFragmentCode.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		CompileLinkErrors(vertexShader, "VERTEX");

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		CompileLinkErrors(fragmentShader, "FRAGMENT");

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, fragmentShader);
		glLinkProgram(m_ID);
		CompileLinkErrors(m_ID, "PROGRAM");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
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

	void Shader::SetInt(const char* _pUniform, int _iValue)
	{
		glUniform1i(glGetUniformLocation(m_ID, _pUniform), _iValue);
	}

	void Shader::SetFloat(const char* _pUniform, float _fValue)
	{
		glUniform1f(glGetUniformLocation(m_ID, _pUniform), _fValue);
	}

	void Shader::SetFloat3(const char* _pUniform, float _fX, float _fY, float _fZ)
	{
		glUniform3f(glGetUniformLocation(m_ID, _pUniform), _fX, _fY, _fZ);
	}

	void Shader::SetFloat4(const char* _pUniform, float _fX, float _fY, float _fZ, float _fW)
	{
		glUniform4f(glGetUniformLocation(m_ID, _pUniform), _fX, _fY, _fZ, _fW);
	}

	void Shader::SetMatrixFloat4(const char* _pUniform, const glm::mat4& _matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, _pUniform), 1, GL_FALSE, glm::value_ptr(_matrix));
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