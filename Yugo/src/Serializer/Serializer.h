#pragma once
#include "Scene/Components.h"
#include "Scene/Entity.h"

#include <json/json.hpp>


namespace Yugo
{

	class Serializer
	{
	public:
		static void Serialize(EntityTagComponent& entityTag, TransformComponent& transform, nlohmann::json& jsonData);
		static void Deserialize(TransformComponent& transform, const nlohmann::json& jsonData);
		static void SaveToFile(const std::string& fileName);
		static void LoadFromFile(const std::string& filePath);
	};

}