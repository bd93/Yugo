#include "pch.h"
#include "Serializer.h"



namespace Yugo
{


	void Serializer::Serialize(EntityTagComponent& entityTag, TransformComponent& transform, nlohmann::json& jsonData)
	{
		jsonData["Transform"].push_back(
			{
				{
					"EntityTag", 
					{
						{"Name", entityTag.Name},
						{"AssetFilePath", entityTag.AssetFilePath}
					}
				},
				{
					"Position",
					{
						{"x", transform.Position.x},
						{"y", transform.Position.y},
						{"z", transform.Position.z}
					}
				},
				{
					"Rotation",
					{
						{"x", transform.Rotation.x},
						{"y", transform.Rotation.y},
						{"z", transform.Rotation.z}
					}
				},
				{
					"Scale",
					{
						{"x", transform.Scale.x},
						{"y", transform.Scale.y},
						{"z", transform.Scale.z}
					}
				}
			}
		);
	}

	void Serializer::Deserialize(TransformComponent& transform, const nlohmann::json& jsonData)
	{
	}

	void Serializer::SaveToFile(const std::string& fileName)
	{
	}

	void Serializer::LoadFromFile(const std::string& filePath)
	{
	}

}