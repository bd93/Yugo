#pragma once
#include "GameObjectInterface.h"
#include "Scene/Scene.h"


namespace Yugo
{

	class GameObjectInterfaceImpl : public GameObjectInterface
	{
	public:
		virtual void SetScene(Scene* scene) override;
		virtual Scene* GetScene() override;

		virtual TransformComponent& GetTransformComponent(Entity& entity) override;
		virtual MeshComponent& GetMeshComponent(Entity& entity) override;
		virtual AnimationComponent& GetAnimationComponent(Entity& entity) override;
		virtual EntityTagComponent& GetEntityTagComponent(Entity& entity) override;

		virtual bool HasTransformComponent(Entity& entity) override;
		virtual bool HasMeshComponent(Entity& entity) override;
		virtual bool HasAnimationComponent(Entity& entity) override;

	private:
		Scene* m_Scene;
	};

}
