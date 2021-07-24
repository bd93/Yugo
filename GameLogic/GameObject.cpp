#include "pch.h"
#include "GameObject.h"



namespace GameLogic
{

	void GameObject::SetEntity(entt::entity entity)
	{
		m_Entity = entity;
	}

	GameObject GameObject::FindGameObjectWithTagName(const std::string& tagName)
	{
		auto gameObjects = FindGameObjectsWithComponent<EntityTagComponent>();

		for (auto& gameObject : gameObjects)
		{
			if (gameObject.GetComponent<EntityTagComponent>().Name == tagName)
				return gameObject;
		}
	}

	std::vector<GameObject> GameObject::GetGameObjectsWithComponent(std::function<std::vector<entt::entity>(void)> findEntities)
	{
		std::vector<GameObject> gameObjects;

		auto& entities = findEntities();
		for (auto entity : entities)
		{
			GameObject gameObject;
			gameObject.SetEntity(entity);
			gameObjects.push_back(gameObject);
		}
		return gameObjects;
	}

	GameObject GameObject::GetGameObjectWithComponent(std::function<entt::entity(void)> findEntities)
	{
		auto entity = findEntities();

		GameObject gameObject;
		gameObject.SetEntity(entity);

		return gameObject;
	}


	extern "C"
	{
		/**
		 * @brief Create game object
		 *
		 * This function is exported to Yugo::ScriptEngine.
		 * Engine creates game object and attach it to script, 
		 * which is also created via exported function.
		 */
		SCRIPT_API GameObject* CreateGameObject(entt::entity entity)
		{
			GameObject* gameObject = new GameObject();
			gameObject->SetEntity(entity);
			return gameObject;
		}

		/**
		 * @brief Destroy game object
		 *
		 * This function is exported to Yugo::ScriptEngine.
		 * Engine destroys game object that are attached to script.
		 * It is called when scripts are stopped in world editor.
		 */
		SCRIPT_API void DeleteGameObject(GameObject* gameObject)
		{
			delete gameObject;
		}
	}

}

