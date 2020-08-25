#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Yugo
{

	class Shader
	{
	private:
		unsigned int m_ShaderID;
		std::string GetShaderSource(std::string filePath);
		void CheckCompileErrors(unsigned int shader, std::string type);
	public:
		Shader(std::string vertexFilePath, std::string fragmentFilePath);
		Shader() = default;
		~Shader();
		void Create(std::string vertexFilePath, std::string fragmentFilePath);
		void Use() const;
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetVec2(const std::string& name, const glm::vec2 & value) const;
		void SetVec3(const std::string& name, const glm::vec3 & value) const;
		void SetVec4(const std::string& name, const glm::vec4 & value) const;
		void SetMat2(const std::string& name, const glm::mat2 & mat) const;
		void SetMat3(const std::string& name, const glm::mat3 & mat) const;
		void SetMat4(const std::string& name, const glm::mat4 & mat) const;
	};

}