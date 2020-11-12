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

		auto view = m_Scene->GetView<EntityTagComponent>();
		for (auto entity : view)
			entities.push_back(entity);

		return entities;
	}

	std::vector<entt::entity>& GameObjectInterfaceImpl::GetEntitiesWithButtonWidgetComponent()
	{
		static std::vector<entt::entity> entities; // static because it can't return heap allocated obj from static lib to dll
		// Clear already used entities
		if (entities.size() > 0)
			entities.clear();

		auto view = m_Scene->GetView<ButtonWidgetComponent>();
		for (auto entity : view)
			entities.push_back(entity);

		return entities;
	}

	std::vector<entt::entity>& GameObjectInterfaceImpl::GetEntitiesWithCanvasWidgetComponent()
	{
		static std::vector<entt::entity> entities; // static because it can't return heap allocated obj from static lib to dll
		// Clear already used entities
		if (entities.size() > 0)
			entities.clear();

		auto view = m_Scene->GetView<CanvasWidgetComponent>();
		for (auto entity : view)
			entities.push_back(entity);

		return entities;
	}

	std::vector<entt::entity>& GameObjectInterfaceImpl::GetEntitiesWithTextWidgetComponent()
	{
		static std::vector<entt::entity> entities; // static because it can't return heap allocated obj from static lib to dll
		// Clear already used entities
		if (entities.size() > 0)
			entities.clear();

		auto view = m_Scene->GetView<TextWidgetComponent>();
		for (auto entity : view)
			entities.push_back(entity);

		return entities;
	}

	//std::vector<GameLogic::GameObject>& GameObjectInterfaceImpl::GetGameObjectsWithEntityTagComponent()
	//{
	//	static std::vector<GameLogic::GameObject> gameObjects; // static because it can't return heap allocated obj from static lib to dll
	//	// Clear already used entities
	//	if (gameObjects.size() > 0)
	//		gameObjects.clear();

	//	GameLogic::GameObject gameObject;

	//	auto view = m_Scene->GetView<EntityTagComponent>();
	//	for (auto entity : view)
	//	{
	//		gameObject.SetEntity(entity);
	//		gameObjects.push_back(gameObject);
	//	}

	//	return gameObjects;
	//}

	//std::vector<GameLogic::GameObject>& GameObjectInterfaceImpl::GetGameObjectsWithButtonWidgetComponent()
	//{
	//	static std::vector<GameLogic::GameObject> gameObjects; // static because it can't return heap allocated obj from static lib to dll
	//	// Clear already used entities
	//	if (gameObjects.size() > 0)
	//		gameObjects.clear();

	//	GameLogic::GameObject gameObject;

	//	auto view = m_Scene->GetView<ButtonWidgetComponent>();
	//	for (auto entity : view)
	//	{
	//		gameObject.SetEntity(entity);
	//		gameObjects.push_back(gameObject);
	//	}

	//	return gameObjects;
	//}

	//std::vector<GameLogic::GameObject>& GameObjectInterfaceImpl::GetGameObjectsWithCanvasWidgetComponent()
	//{
	//	static std::vector<GameLogic::GameObject> gameObjects; // static because it can't return heap allocated obj from static lib to dll
	//	// Clear already used entities
	//	if (gameObjects.size() > 0)
	//		gameObjects.clear();

	//	GameLogic::GameObject gameObject;

	//	auto view = m_Scene->GetView<CanvasWidgetComponent>();
	//	for (auto entity : view)
	//	{
	//		gameObject.SetEntity(entity);
	//		gameObjects.push_back(gameObject);
	//	}

	//	return gameObjects;
	//}

	//std::vector<GameLogic::GameObject>& GameObjectInterfaceImpl::GetGameObjectsWithTextWidgetComponent()
	//{
	//	static std::vector<GameLogic::GameObject> gameObjects; // static because it can't return heap allocated obj from static lib to dll
	//	// Clear already used entities
	//	if (gameObjects.size() > 0)
	//		gameObjects.clear();

	//	GameLogic::GameObject gameObject;

	//	auto view = m_Scene->GetView<TextWidgetComponent>();
	//	for (auto entity : view)
	//	{
	//		gameObject.SetEntity(entity);
	//		gameObjects.push_back(gameObject);
	//	}

	//	return gameObjects;
	//}

	TransformComponent& GameObjectInterfaceImpl::GetTransformComponent(entt::entity entity)
	{
		return m_Scene->GetComponent<TransformComponent>(entity);
	}

	MeshComponent& GameObjectInterfaceImpl::GetMeshComponent(entt::entity entity)
	{
		return m_Scene->GetComponent<MeshComponent>(entity);
	}

	AnimationComponent& GameObjectInterfaceImpl::GetAnimationComponent(entt::entity entity)
	{
		return m_Scene->GetComponent<AnimationComponent>(entity);
	}

	EntityTagComponent& GameObjectInterfaceImpl::GetEntityTagComponent(entt::entity entity)
	{
		return m_Scene->GetComponent<EntityTagComponent>(entity);
	}

	ScriptComponent& GameObjectInterfaceImpl::GetScriptComponent(entt::entity entity)
	{
		return m_Scene->GetComponent<ScriptComponent>(entity);
	}

	CameraComponent& GameObjectInterfaceImpl::GetCameraComponent(entt::entity entity)
	{
		return m_Scene->GetComponent<CameraComponent>(entity);
	}

	std::vector<CanvasWidgetComponent*>& GameObjectInterfaceImpl::GetCanvasWidgetComponentsInChildren(entt::entity entity)
	{
		static std::vector<CanvasWidgetComponent*> components;
		// Clear already used components
		if (components.size() > 0)
			components.clear();

		TraverseFun traverse = [&](entt::entity node) {
			auto& relationship = m_Scene->GetComponent<RelationshipComponent>(node);
			for (auto child : relationship.Children)
			{
				if (m_Scene->HasComponent<CanvasWidgetComponent>(child))
				{
					auto& canvas = m_Scene->GetComponent<CanvasWidgetComponent>(child);
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
			auto& relationship = m_Scene->GetComponent<RelationshipComponent>(node);
			for (auto child : relationship.Children)
			{
				if (m_Scene->HasComponent<ButtonWidgetComponent>(child))
				{
					auto& button = m_Scene->GetComponent<ButtonWidgetComponent>(child);
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
			auto& relationship = m_Scene->GetComponent<RelationshipComponent>(node);
			for (auto child : relationship.Children)
			{
				if (m_Scene->HasComponent<TextWidgetComponent>(child))
				{
					auto& text = m_Scene->GetComponent<TextWidgetComponent>(child);
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
			auto& relationship = m_Scene->GetComponent<RelationshipComponent>(node);
			for (auto child : relationship.Children)
			{
				if (m_Scene->HasComponent<TransformComponent>(child))
				{
					auto& transform = m_Scene->GetComponent<TransformComponent>(child);
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
		return m_Scene->HasComponent<TransformComponent>(entity);
	}

	bool GameObjectInterfaceImpl::HasMeshComponent(entt::entity entity)
	{
		return m_Scene->HasComponent<MeshComponent>(entity);
	}

	bool GameObjectInterfaceImpl::HasAnimationComponent(entt::entity entity)
	{
		return m_Scene->HasComponent<AnimationComponent>(entity);
	}

	bool GameObjectInterfaceImpl::HasEntityTagComponent(entt::entity entity)
	{
		return m_Scene->HasComponent<EntityTagComponent>(entity);
	}

	bool GameObjectInterfaceImpl::HasScriptComponent(entt::entity entity)
	{
		return m_Scene->HasComponent<ScriptComponent>(entity);
	}

	bool GameObjectInterfaceImpl::HasCameraComponent(entt::entity entity)
	{
		return m_Scene->HasComponent<CameraComponent>(entity);
	}

}

