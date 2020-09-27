#pragma once
#include "GameObjectInterface.h"
#include "Components.h"
#include "Scene/Entity.h"


/*
This class represents scriptable game object (entity);
Each script is attached to only one game object;
*/
class GameObject
{
public:
	GameObject(GameObjectInterface* gameObjectInterface = nullptr);

	void SetEntity(entt::entity entity);

	template<typename T>
	T& GetComponent() {}
	template<>
	TransformComponent& GetComponent<TransformComponent>() { return s_GameObjectInterface->GetTransformComponent(m_Entity); }
	template<>
	MeshComponent& GetComponent<MeshComponent>() { return s_GameObjectInterface->GetMeshComponent(m_Entity); }
	template<>
	AnimationComponent& GetComponent<AnimationComponent>() { return s_GameObjectInterface->GetAnimationComponent(m_Entity); }
	template<>
	EntityTagComponent& GetComponent<EntityTagComponent>() { return s_GameObjectInterface->GetEntityTagComponent(m_Entity); }

	template<typename T>
	bool HasComponent() {}
	template<>
	bool HasComponent<TransformComponent>() { return s_GameObjectInterface->HasTransformComponent(m_Entity); }
	template<>
	bool HasComponent<MeshComponent>() { return s_GameObjectInterface->HasMeshComponent(m_Entity); }
	template<>
	bool HasComponent<AnimationComponent>() { return s_GameObjectInterface->HasAnimationComponent(m_Entity); }
	template<>
	bool HasComponent<EntityTagComponent>() { return s_GameObjectInterface->HasEntityTagComponent(m_Entity); }

	template<typename T>
	std::vector<T*>& GetComponentsInChildren() {}
	template<>
	std::vector<CanvasWidgetComponent*>& GetComponentsInChildren() { return s_GameObjectInterface->GetCanvasWidgetComponentsInChildren(m_Entity); }
	template<>
	std::vector<ButtonWidgetComponent*>& GetComponentsInChildren() { return s_GameObjectInterface->GetButtonWidgetComponentsInChildren(m_Entity); }
	template<>
	std::vector<TextWidgetComponent*>& GetComponentsInChildren() { return s_GameObjectInterface->GetTextWidgetComponentsInChildren(m_Entity); }
	template<>
	std::vector<TransformComponent*>& GetComponentsInChildren() { return s_GameObjectInterface->GetTransformComponentsInChildren(m_Entity); }

	static std::vector<GameObject> FindGameObjectsWithTag(const std::string& tagName);
	
private:
	std::string m_Name;
	entt::entity m_Entity;

	static GameObjectInterface* s_GameObjectInterface;
};