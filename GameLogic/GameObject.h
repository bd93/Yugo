#pragma once
#include "GameObjectInterface.h"
#include "Components.h"


/*
This class represents scriptable game object (entity);
Each script is attached to only one game object;
*/
class GameObject
{
public:
	GameObject(GameObjectInterface* gameObjectInterface = nullptr);

	void SetEntity(Yugo::Entity& entity);

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

	static std::vector<GameObject> FindGameObjectsWithTag(const std::string& tagName);
	
private:
	std::string m_Name;
	Yugo::Entity m_Entity;

	static GameObjectInterface* s_GameObjectInterface;
};