#pragma once
#include "Scene/Entity.h" // TODO: Fix this! Try to implement something else because of possible circular dep.
#include "Animation/AnimationComponent.h"
#include "Scene/Components.h"

/*
This class represents interface for accessing game engine functionalities;
Each client script holds pointer to this interface;
Methods are overridden on the game engine side;
*/
class Behaviour
{
public:
	virtual ~Behaviour() { delete m_Entity; }

	virtual bool IsKeyboardkeyPressed(Yugo::KeyCode key) = 0;
	virtual bool IsMouseButtonPressed(Yugo::MouseCode button) = 0;

	virtual Yugo::Entity* GetEntity() = 0;
	virtual void SetEntity(Yugo::Entity* entity) = 0;

	virtual Yugo::TransformComponent& GetTransformComponent() = 0;
	virtual Yugo::MeshComponent& GetMeshComponent() = 0;
	virtual Yugo::AnimationComponent& GetAnimationComponent() = 0;

	virtual bool HasTransformComponent() = 0;
	virtual bool HasMeshComponent() = 0;
	virtual bool HasAnimationComponent() = 0;

protected:
	Yugo::Entity* m_Entity; // Game object
};