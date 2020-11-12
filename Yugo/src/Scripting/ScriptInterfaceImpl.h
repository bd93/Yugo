#pragma once
#include "GameObject.h"
#include "Interfaces/ScriptInterface.h"
#include "AllComponents.h"

namespace Yugo
{

	//class ScriptInterfaceImpl : public GameLogic::ScriptInterface
	//{
	//public:
	//	virtual bool IsKeyboardkeyPressed(Yugo::KeyCode key) override;
	//	virtual bool IsMouseButtonPressed(Yugo::MouseCode button) override;
	//	virtual bool IsMouseHoveringRect(TransformComponent& transform) override;

	//	virtual Yugo::Entity& GetEntity() override;
	//	virtual void SetEntity(Yugo::Entity& entity) override;

	//	virtual TransformComponent& GetTransformComponent() override;
	//	virtual MeshComponent& GetMeshComponent() override;
	//	virtual AnimationComponent& GetAnimationComponent() override;
	//	virtual EntityTagComponent& GetEntityTagComponent() override;
	//	virtual SpriteComponent& GetSpriteComponent() override;
	//	virtual ScriptComponent& GetScriptComponent() override;

	//	virtual bool HasTransformComponent() override;
	//	virtual bool HasMeshComponent() override;
	//	virtual bool HasAnimationComponent() override;
	//	virtual bool HasEntityTagComponent() override;
	//	virtual bool HasSpriteComponent() override;
	//	virtual bool HasScriptComponent() override;

	//private:
	//	Entity m_Entity;
	//};

}