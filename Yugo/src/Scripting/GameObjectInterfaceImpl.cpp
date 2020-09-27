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
		static std::vector<entt::entity> entities; // static because it can't return heap allocated obj from static lib to dll
		// Clear already used entities
		if (entities.size() > 0)
			entities.clear();

		auto view = m_Scene->GetRegistry().view<EntityTagComponent>();
		for (auto entity : view)
			entities.push_back(entity);

		return entities;
	}

	TransformComponent& GameObjectInterfaceImpl::GetTransformComponent(entt::entity entity)
	{
		return m_Scene->GetRegistry().get<TransformComponent>(entity);
	}

	MeshComponent& GameObjectInterfaceImpl::GetMeshComponent(entt::entity entity)
	{
		return m_Scene->GetRegistry().get<MeshComponent>(entity);
	}

	AnimationComponent& GameObjectInterfaceImpl::GetAnimationComponent(entt::entity entity)
	{
		return m_Scene->GetRegistry().get<AnimationComponent>(entity);
	}

	EntityTagComponent& GameObjectInterfaceImpl::GetEntityTagComponent(entt::entity entity)
	{
		return m_Scene->GetRegistry().get<EntityTagComponent>(entity);
	}

	std::vector<CanvasWidgetComponent*>& GameObjectInterfaceImpl::GetCanvasWidgetComponentsInChildren(entt::entity entity)
	{
		static std::vector<CanvasWidgetComponent*> components;
		// Clear already used components
		if (components.size() > 0)
			components.clear();

		TraverseFun traverse = [&](entt::entity node) {
			auto& relationship = m_Scene->GetRegistry().get<RelationshipComponent>(node);
			for (auto child : relationship.Children)
			{
				if (m_Scene->GetRegistry().has<CanvasWidgetComponent>(child))
				{
					auto& canvas = m_Scene->GetRegistry().get<CanvasWidgetComponent>(child);
					components.push_back(&canvas);
				}
			}

			for (auto child : relationship.Children)
			{
				traverse(child);
			}
		};

		traverse(entity);

		return components;
	}

	std::vector<ButtonWidgetComponent*>& GameObjectInterfaceImpl::GetButtonWidgetComponentsInChildren(entt::entity entity)
	{
		static std::vector<ButtonWidgetComponent*> components;
		// Clear already used components
		if (components.size() > 0)
			components.clear();

		TraverseFun traverse = [&](entt::entity node) {
			auto& relationship = m_Scene->GetRegistry().get<RelationshipComponent>(node);
			for (auto child : relationship.Children)
			{
				if (m_Scene->GetRegistry().has<ButtonWidgetComponent>(child))
				{
					auto& button = m_Scene->GetRegistry().get<ButtonWidgetComponent>(child);
					components.push_back(&button);
				}
			}

			for (auto child : relationship.Children)
			{
				traverse(child);
			}
		};

		traverse(entity);

		return components;
	}

	std::vector<TextWidgetComponent*>& GameObjectInterfaceImpl::GetTextWidgetComponentsInChildren(entt::entity entity)
	{
		static std::vector<TextWidgetComponent*> components;
		// Clear already used components
		if (components.size() > 0)
			components.clear();

		TraverseFun traverse = [&](entt::entity node) {
			auto& relationship = m_Scene->GetRegistry().get<RelationshipComponent>(node);
			for (auto child : relationship.Children)
			{
				if (m_Scene->GetRegistry().has<TextWidgetComponent>(child))
				{
					auto& text = m_Scene->GetRegistry().get<TextWidgetComponent>(child);
					components.push_back(&text);
				}
			}

			for (auto child : relationship.Children)
			{
				traverse(child);
			}
		};

		traverse(entity);

		return components;
	}

	std::vector<TransformComponent*>& GameObjectInterfaceImpl::GetTransformComponentsInChildren(entt::entity entity)
	{
		static std::vector<TransformComponent*> components;
		// Clear already used components
		if (components.size() > 0)
			components.clear();

		TraverseFun traverse = [&](entt::entity node) {
			auto& relationship = m_Scene->GetRegistry().get<RelationshipComponent>(node);
			for (auto child : relationship.Children)
			{
				if (m_Scene->GetRegistry().has<TransformComponent>(child))
				{
					auto& transform = m_Scene->GetRegistry().get<TransformComponent>(child);
					components.push_back(&transform);
				}
			}

			for (auto child : relationship.Children)
			{
				traverse(child);
			}
		};

		traverse(entity);

		return components;
	}

	bool GameObjectInterfaceImpl::HasTransformComponent(entt::entity entity)
	{
		return m_Scene->GetRegistry().has<TransformComponent>(entity);
	}

	bool GameObjectInterfaceImpl::HasMeshComponent(entt::entity entity)
	{
		return m_Scene->GetRegistry().has<MeshComponent>(entity);
	}

	bool GameObjectInterfaceImpl::HasAnimationComponent(entt::entity entity)
	{
		return m_Scene->GetRegistry().has<AnimationComponent>(entity);
	}

	bool GameObjectInterfaceImpl::HasEntityTagComponent(entt::entity entity)
	{
		return m_Scene->GetRegistry().has<EntityTagComponent>(entity);
	}

}

