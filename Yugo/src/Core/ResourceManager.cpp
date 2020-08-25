#include "pch.h"
#include "ResourceManager.h"


namespace Yugo
{

	std::unordered_map<std::string, Shader> ResourceManager::s_ShaderMap;
	std::unordered_map<std::string, Texture> ResourceManager::s_TextureMap;

	void ResourceManager::AddShader(std::string name, Shader& shader)
	{
		s_ShaderMap[name] = shader;
	}

	void ResourceManager::AddTexture(std::string fileName, Texture& texture)
	{
		s_TextureMap[fileName] = texture;
	}

	const Shader& ResourceManager::GetShader(std::string name)
	{
		return s_ShaderMap[name];
	}

	const Texture& ResourceManager::GetTexture(std::string fileName)
	{
		return s_TextureMap[fileName];
	}

}