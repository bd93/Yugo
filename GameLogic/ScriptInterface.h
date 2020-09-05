#pragma once
#include "Scene/Entity.h"
#include "Animation/AnimationComponent.h"
#include "Scene/Components.h"
#include "Components.h"

/*
This class represents interface for accessing game engine functionalities;
Each client script holds pointer to this interface;
Methods are overridden on the game engine side;
*/
class ScriptInterface
{
public:
	virtual bool IsKeyboardkeyPressed(Yugo::KeyCode key) = 0;
	virtual bool IsMouseButtonPressed(Yugo::MouseCode button) = 0;

	virtual Yugo::Entity& GetEntity() = 0;
	virtual void SetEntity(Yugo::Entity& entity) = 0;

	virtual TransformComponent& GetTransformComponent() = 0;
	virtual MeshComponent& GetMeshComponent() = 0;
	virtual AnimationComponent& GetAnimationComponent() = 0;
	virtual EntityTagComponent& GetEntityTagComponent() = 0;

	virtual bool HasTransformComponent() = 0;
	virtual bool HasMeshComponent() = 0;
	virtual bool HasAnimationComponent() = 0;
};