#pragma once
#include "YugoComponents.h"
//#include "Scene/Entity.h"
#include "EngineFuncs.h"
#include "Common.h"


namespace GameLogic
{

	/* @brief Scriptable game object on the scene. */
	class GameObject
	{
	public:
		void SetEntity(entt::entity entity);

		template<typename ComponentType>
		ComponentType& GetComponent()
		{
		}

		template<>
		TransformComponent& GetComponent<TransformComponent>()
		{
			return EngineFuncs::GetGameEngineFuncs().GetTransformComponent(m_Entity);
		}


		//static void SetInterface(GameObjectInterface* gameObjectInterface);

		//static std::vector<GameObject> FindGameObjectsWithTag(const std::string& tagName);
		//static GameObject FindGameObjectWithTag(const std::string& tagName);

		///* @brief Returns component that is attached to this game object. */
		//template<typename ComponentType>
		//ComponentType& GetComponent() {}
		//template<>
		//TransformComponent& GetComponent<TransformComponent>() { return s_GameObjectInterface->GetTransformComponent(m_Entity); }
		//template<>
		//MeshComponent& GetComponent<MeshComponent>() { return s_GameObjectInterface->GetMeshComponent(m_Entity); }
		//template<>
		//AnimationComponent& GetComponent<AnimationComponent>() { return s_GameObjectInterface->GetAnimationComponent(m_Entity); }
		//template<>
		//EntityTagComponent& GetComponent<EntityTagComponent>() { return s_GameObjectInterface->GetEntityTagComponent(m_Entity); }
		//template<>
		//ScriptComponent& GetComponent<ScriptComponent>() { return s_GameObjectInterface->GetScriptComponent(m_Entity); }
		//template<>
		//CameraComponent& GetComponent<CameraComponent>() { return s_GameObjectInterface->GetCameraComponent(m_Entity); }

		///* @brief Checks if this game object has component attached to it. */
		//template<typename ComponentType>
		//bool HasComponent() {}
		//template<>
		//bool HasComponent<TransformComponent>() { return s_GameObjectInterface->HasTransformComponent(m_Entity); }
		//template<>
		//bool HasComponent<MeshComponent>() { return s_GameObjectInterface->HasMeshComponent(m_Entity); }
		//template<>
		//bool HasComponent<AnimationComponent>() { return s_GameObjectInterface->HasAnimationComponent(m_Entity); }
		//template<>
		//bool HasComponent<EntityTagComponent>() { return s_GameObjectInterface->HasEntityTagComponent(m_Entity); }
		//template<>
		//bool HasComponent<ScriptComponent>() { return s_GameObjectInterface->HasScriptComponent(m_Entity); }
		//template<>
		//bool HasComponent<CameraComponent>() { return s_GameObjectInterface->HasCameraComponent(m_Entity); }

		///* @brief Returns components which are attached to children of this game object. */
		//template<typename ComponentType>
		//std::vector<ComponentType*>& GetComponentsInChildren() {}
		//template<>
		//std::vector<CanvasWidgetComponent*>& GetComponentsInChildren() { return s_GameObjectInterface->GetCanvasWidgetComponentsInChildren(m_Entity); }
		//template<>
		//std::vector<ButtonWidgetComponent*>& GetComponentsInChildren() { return s_GameObjectInterface->GetButtonWidgetComponentsInChildren(m_Entity); }
		//template<>
		//std::vector<TextWidgetComponent*>& GetComponentsInChildren() { return s_GameObjectInterface->GetTextWidgetComponentsInChildren(m_Entity); }
		//template<>
		//std::vector<TransformComponent*>& GetComponentsInChildren() { return s_GameObjectInterface->GetTransformComponentsInChildren(m_Entity); }

		///* @brief Returns game objects that have given component. */
		//template<typename ComponentType>
		//static std::vector<GameObject> FindGameObjectsWithComponent() {}
		//template<>
		//static std::vector<GameObject> FindGameObjectsWithComponent<EntityTagComponent>()
		//{
		//	std::vector<GameObject> gameObjects;
		//	auto& entities = s_GameObjectInterface->GetEntitiesWithEntityTagComponent();
		//	for (auto entity : entities)
		//	{
		//		GameObject gameObject;
		//		gameObject.SetEntity(entity);
		//		gameObjects.push_back(gameObject);
		//	}
		//	return gameObjects;
		//}
		//template<>
		//static std::vector<GameObject> FindGameObjectsWithComponent<ButtonWidgetComponent>()
		//{
		//	std::vector<GameObject> gameObjects;
		//	auto& entities = s_GameObjectInterface->GetEntitiesWithButtonWidgetComponent();
		//	for (auto entity : entities)
		//	{
		//		GameObject gameObject;
		//		gameObject.SetEntity(entity);
		//		gameObjects.push_back(gameObject);
		//	}
		//	return gameObjects;
		//}
	
	private:
		std::string m_Name;
		entt::entity m_Entity;

		//static GameObjectInterface* s_GameObjectInterface;
	};

}