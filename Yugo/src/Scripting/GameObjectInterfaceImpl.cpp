#include "pch.h"
#include "GameObjectInterfaceImpl.h"


namespace Yugo
{

	void GameObjectInterfaceImpl::SetScene(Scene* scene)
	{
		m_Scene = scene;
	}

	Scene* GameObjectInterfaceImpl::GetScene()
	{
		return m_Scene;
	}

	TransformComponent& GameObjectInterfaceImpl::GetTransformComponent(Entity& entity)
	{
		return entity.GetComponent<TransformComponent>();
	}

	MeshComponent& GameObjectInterfaceImpl::GetMeshComponent(Entity& entity)
	{
		return entity.GetComponent<MeshComponent>();
	}

	AnimationComponent& GameObjectInterfaceImpl::GetAnimationComponent(Entity& entity)
	{
		return entity.GetComponent<AnimationComponent>();
	}

	EntityTagComponent& GameObjectInterfaceImpl::GetEntityTagComponent(Entity& entity)
	{
		return entity.GetComponent<EntityTagComponent>();
	}

	bool GameObjectInterfaceImpl::HasTransformComponent(Entity& entity)
	{
		return entity.HasComponent<TransformComponent>();
	}

	bool GameObjectInterfaceImpl::HasMeshComponent(Entity& entity)
	{
		return entity.HasComponent<MeshComponent>();
	}

	bool GameObjectInterfaceImpl::HasAnimationComponent(Entity& entity)
	{
		return entity.HasComponent<AnimationComponent>();
	}

}

