#include "pch.h"
#include "GameObject.h"


#define SCRIPT_API __declspec(dllexport)


GameObjectInterface* GameObject::s_GameObjectInterface = nullptr;

GameObject::GameObject(GameObjectInterface* gameObjectInterface)
{
	if (gameObjectInterface != nullptr)
		s_GameObjectInterface = gameObjectInterface;
}

void GameObject::SetEntity(Yugo::Entity& entity)
{
	m_Entity = entity;
}

std::vector<GameObject> GameObject::FindGameObjectsWithTag(const std::string& tagName)
{
	std::vector<GameObject> gameObjects; // TODO: If this function is going to be used frequently (e.g. in OnUpdate()), then refactor this!

	auto scene = s_GameObjectInterface->GetScene();

	auto& entities = s_GameObjectInterface->GetEntitiesWithEntityTagComponent();
	for (auto& entity : entities)
	{
		GameObject gameObject;
		gameObject.SetEntity(Yugo::Entity(entity, tagName, scene));
		auto& entityTagComponent = gameObject.GetComponent<EntityTagComponent>();

		if (entityTagComponent.Name == tagName)
			gameObjects.push_back(gameObject);
	}

	return gameObjects;
}


extern "C"
{
	SCRIPT_API GameObject* CreateGameObject(GameObjectInterface* gameObjectInterface)
	{
		GameObject* gameObject = new GameObject(gameObjectInterface);
		return gameObject;
	}

	SCRIPT_API void DeleteGameObject(GameObject* gameObject)
	{
		delete gameObject;
	}
}