#include "pch.h"
#include "Texture.h"
#include "core/Common.h"


namespace Yugo
{

	void Texture::Create(const std::string& fullPath)
	{
		glGenTextures(1, &m_TextureID);
		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		int width;
		int height;
		int nrChannels;
		//stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			GLenum format;
			switch (nrChannels)
			{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				format = GL_RGBA;
				break;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE/n";
		}

		Unbind();

		stbi_image_free(data);
	}

	Texture::Texture(const std::string& fileName, const std::string& directory, const std::string& type)
		: m_FileName(fileName), m_Type(type)
	{
		std::string fullPath = directory + "\\" + fileName;
		Create(fullPath);
	}

	Texture::Texture(const std::string& textureFilePath, const std::string& type)
		: m_Type(type)
	{
		Create(textureFilePath);
	}

	Texture::Texture(const std::string& textureFilePath)
	{
		Create(textureFilePath);
	}

	// Texture for Frame Buffer
	Texture::Texture(unsigned int width, unsigned int height)
	{
		// By default create Texture Buffer with NULL data
		// Usefull for Frame Buffer texture attachment
		glGenTextures(1, &m_TextureID);
		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Default Texture Buffer
		/*
		* Important!
		* If @internal_format parameter is changed to GL_RGBA, then nanosuit model has transparent textures, which are white-ish on GL_RGB spec
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		Unbind();
	}

	Texture::Texture(unsigned int width, unsigned int height, int numOfSamples)
	{
		glGenTextures(1, &m_TextureID);
		BindMultisampled();
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numOfSamples, GL_RGB, width, height, GL_TRUE);
		UnbindMultisampled();
	}

	const unsigned int Texture::GetId() const
	{
		return m_TextureID;
	}

	const std::string& Texture::GetType() const
	{
		return m_Type;
	}

	const std::string& Texture::GetFileName() const
	{
		return m_FileName;
	}

	void Texture::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture::BindMultisampled() const
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureID);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::UnbindMultisampled() const
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}

	void Texture::Resize(int width, int height)
	{
		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		Unbind();
	}

	void Texture::ResizeMultisampled(int width, int height, int numOfSamples)
	{
		BindMultisampled();
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numOfSamples, GL_RGB, width, height, GL_TRUE);
		UnbindMultisampled();
	}

}