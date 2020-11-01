#pragma once
#include "GameObjectInterface.h"
#include "Scene/Scene.h"


namespace Yugo
{

	class GameObjectInterfaceImpl : public GameLogic::GameObjectInterface
	{
		using TraverseFun = std::function<void(entt::entity)>;

	public:
		virtual void SetScene(Scene* scene) override;
		virtual Scene* GetScene() override;

		virtual std::vector<entt::entity>& GetEntitiesWithEntityTagComponent() override;

		virtual TransformComponent& GetTransformComponent(entt::entity entity) override;
		virtual MeshComponent& GetMeshComponent(entt::entity entity) override;
		virtual AnimationComponent& GetAnimationComponent(entt::entity entity) override;
		virtual EntityTagComponent& GetEntityTagComponent(entt::entity entity) override;

		virtual std::vector<CanvasWidgetComponent*>& GetCanvasWidgetComponentsInChildren(entt::entity entity) override;
		virtual std::vector<ButtonWidgetComponent*>& GetButtonWidgetComponentsInChildren(entt::entity entity) override;
		virtual std::vector<TextWidgetComponent*>& GetTextWidgetComponentsInChildren(entt::entity entity) override;
		virtual std::vector<TransformComponent*>& GetTransformComponentsInChildren(entt::entity entity) override;

		virtual bool HasTransformComponent(entt::entity entity) override;
		virtual bool HasMeshComponent(entt::entity entity) override;
		virtual bool HasAnimationComponent(entt::entity entity) override;
		virtual bool HasEntityTagComponent(entt::entity entity) override;

	private:
		Scene* m_Scene;
	};

}
