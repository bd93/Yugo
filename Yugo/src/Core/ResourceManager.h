#pragma once
#include <unordered_map>
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Core/Core.h"

namespace Yugo
{

	struct ResourceManager
	{
		ResourceManager() = delete;
		static std::unordered_map<std::string, Shader> s_ShaderMap;
		static std::unordered_map<std::string, Texture> s_TextureMap;
		static void AddShader(std::string name, Shader& shader);
		static void AddTexture(std::string name, Texture& texture);
		static const Shader& GetShader(std::string name);
		static const Texture& GetTexture(std::string name);
	};

}