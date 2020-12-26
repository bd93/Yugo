#include "pch.h"
#include "GameObject.h"


#define SCRIPT_API __declspec(dllexport)

namespace GameLogic
{

	//GameObjectInterface* GameObject::s_GameObjectInterface = nullptr;

	//void GameObject::SetEntity(entt::entity entity)
	//{
	//	m_Entity = entity;
	//}

	///* @brief Returns game objects with given tag name. */
	//std::vector<GameObject> GameObject::FindGameObjectsWithTag(const std::string& tagName)
	//{
	//	std::vector<GameObject> gameObjects; // TODO: If this function is going to be used frequently (e.g. in OnUpdate()), then refactor this!

	//	auto scene = s_GameObjectInterface->GetScene();

	//	auto& entities = s_GameObjectInterface->GetEntitiesWithEntityTagComponent();
	//	for (auto& entity : entities)
	//	{
	//		GameObject gameObject;
	//		gameObject.SetEntity(entity);
	//		auto& entityTagComponent = gameObject.GetComponent<EntityTagComponent>();

	//		if (entityTagComponent.Name == tagName)
	//			gameObjects.push_back(gameObject);
	//	}

	//	return gameObjects;
	//}

	///* @brief Returns first game object with given tag name. */
	//GameObject GameObject::FindGameObjectWithTag(const std::string& tagName)
	//{
	//	GameObject gameObject;

	//	auto scene = s_GameObjectInterface->GetScene();
	//	auto& entities = s_GameObjectInterface->GetEntitiesWithEntityTagComponent();
	//	for (auto& entity : entities)
	//	{
	//		auto& entityTagComponent = s_GameObjectInterface->GetEntityTagComponent(entity);

	//		if (entityTagComponent.Name == tagName)
	//			gameObject.SetEntity(entity);
	//	}

	//	return gameObject;
	//}

	//void GameObject::SetInterface(GameObjectInterface* gameObjectInterface)
	//{
	//	s_GameObjectInterface = gameObjectInterface;
	//}


	//extern "C"
	//{
	//	/**
	//	 * @brief Create game object
	//	 *
	//	 * This function is exported to Yugo::ScriptEngine.
	//	 * Engine creates game object and attach it to script, 
	//	 * which is also created via exported function.
	//	 */
	//	SCRIPT_API GameObject* CreateGameObject(entt::entity entity)
	//	{
	//		GameObject* gameObject = new GameObject();
	//		gameObject->SetEntity(entity);
	//		return gameObject;
	//	}

	//	/**
	//	 * @brief Destroy game object
	//	 *
	//	 * This function is exported to Yugo::ScriptEngine.
	//	 * Engine destroys game object that are attached to script.
	//	 * It is called when scripts are stopped in world editor.
	//	 */
	//	SCRIPT_API void DeleteGameObject(GameObject* gameObject)
	//	{
	//		delete gameObject;
	//	}

	//	//SCRIPT_API void SetGameObjectInterface(GameObjectInterface* gameObjectInterface)
	//	//{
	//	//	GameObject::SetInterface(gameObjectInterface);
	//	//}
	//}

}

