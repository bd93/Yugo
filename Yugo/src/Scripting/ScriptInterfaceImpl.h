#pragma once
#include "ScriptInterface.h"
#include "GameObject.h"


namespace Yugo
{

	class ScriptInterfaceImpl : public ScriptInterface
	{
	public:
		virtual bool IsKeyboardkeyPressed(Yugo::KeyCode key) override;
		virtual bool IsMouseButtonPressed(Yugo::MouseCode button) override;

		virtual Yugo::Entity& GetEntity() override;
		virtual void SetEntity(Yugo::Entity& entity) override;

		virtual TransformComponent& GetTransformComponent() override;
		virtual MeshComponent& GetMeshComponent() override;
		virtual AnimationComponent& GetAnimationComponent() override;
		virtual EntityTagComponent& GetEntityTagComponent() override;

		virtual bool HasTransformComponent() override;
		virtual bool HasMeshComponent() override;
		virtual bool HasAnimationComponent() override;

	private:
		Entity m_Entity;
	};

}