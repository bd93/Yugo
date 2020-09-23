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

	std::vector<entt::entity>& GameObjectInterfaceImpl::GetEntitiesWithEntityTagComponent()
	{
		// Clear already used entities
		if (m_Entities.size() > 0)
			m_Entities.clear();

		auto view = m_Scene->GetRegistry().view<EntityTagComponent>();
		for (auto entity : view)
			m_Entities.push_back(entity);

		return m_Entities;
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

