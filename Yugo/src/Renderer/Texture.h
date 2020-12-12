#pragma once
#include <stb_image.h>


namespace Yugo
{

	class Texture
	{
	private:
		unsigned int m_TextureID;

		std::string m_FileName;
		std::string m_Type;

		void Create(const std::string& fullPath);

	public:
		Texture(const std::string& fileName, const std::string& directory, const std::string& type);
		Texture(const std::string& textureFilePath, const std::string& type);
		Texture(const std::string& textureFilePath);
		Texture(unsigned int width, unsigned int height);
		Texture(unsigned int width, unsigned int height, int numOfSamples);
		Texture() = default;
		const unsigned int GetId() const;
		const std::string& GetType() const;
		const std::string& GetFileName() const;
		void Bind() const;
		void BindMultisampled() const;
		void Unbind() const;
		void UnbindMultisampled() const;
		void Resize(int width, int height);
		void ResizeMultisampled(int width, int height, int numOfSamples);
	};

}