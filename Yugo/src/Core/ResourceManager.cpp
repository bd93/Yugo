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

	bool ResourceManager::HasShader(std::string name)
	{
		if (s_ShaderMap.find(name) != s_ShaderMap.end())
			return true;
		else
			return false;
	}

	bool ResourceManager::HasTexture(std::string name)
	{
		if (s_TextureMap.find(name) != s_TextureMap.end())
			return true;
		else
			return false;
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