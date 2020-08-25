#pragma once
#include "Behaviour.h"



namespace Yugo
{

	class ScriptBehaviour : public Behaviour
	{
	public:
		virtual bool IsKeyboardkeyPressed(Yugo::KeyCode key) override;
		virtual bool IsMouseButtonPressed(Yugo::MouseCode button) override;

		virtual Yugo::Entity* GetEntity() override;
		virtual void SetEntity(Yugo::Entity* entity) override;

		virtual Yugo::TransformComponent& GetTransformComponent() override;
		virtual Yugo::MeshComponent& GetMeshComponent() override;
		virtual Yugo::AnimationComponent& GetAnimationComponent() override;

		virtual bool HasTransformComponent() override;
		virtual bool HasMeshComponent() override;
		virtual bool HasAnimationComponent() override;
	};

}