#include "pch.h"
#include "Core/Common.h"
#include "Shader.h"

namespace Yugo
{

	Shader::Shader(std::string vertexFilePath, std::string fragmentFilePath)
	{
		this->Create(vertexFilePath, fragmentFilePath);
	}

	Shader::~Shader()
	{
	}

	void Shader::Create(std::string vertexFilePath, std::string fragmentFilePath)
	{
		const std::string& vertexShaderCodeString = this->GetShaderSource(vertexFilePath);
		const std::string& fragmentShaderCodeString = this->GetShaderSource(fragmentFilePath);

		const char* vertexShaderCode = vertexShaderCodeString.c_str();
		const char* fragmentShaderCode = fragmentShaderCodeString.c_str();

		unsigned int vertexShader, fragmentShader;

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
		glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
		glCompileShader(vertexShader);
		this->CheckCompileErrors(vertexShader, "VERTEX_SHADER");
		glCompileShader(fragmentShader);
		this->CheckCompileErrors(vertexShader, "FRAGMENT_SHADER");

		m_ShaderID = glCreateProgram();
		glAttachShader(m_ShaderID, vertexShader);
		glAttachShader(m_ShaderID, fragmentShader);
		glLinkProgram(m_ShaderID);
		this->CheckCompileErrors(m_ShaderID, "PROGRAM");
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::Use() const
	{
		glUseProgram(m_ShaderID);
	}

	void Shader::SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), (int)value);
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), value);
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ShaderID, name.c_str()), value);
	}

	void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
	}

	void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
	}

	void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
	}

	void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::CheckCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
			}
		}
	}

	std::string Shader::GetShaderSource(std::string filePath)
	{
		std::string shaderCode;
		std::ifstream inputFileStream;

		inputFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open file
			inputFileStream.open(filePath);
			std::stringstream stringStream;
			// read file's buffer contents into streams
			stringStream << inputFileStream.rdbuf();
			// close file handlers
			inputFileStream.close();
			// convert stream into string
			shaderCode = stringStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		return shaderCode;
	}

}