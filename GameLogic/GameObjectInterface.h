#pragma once

#include "Components.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Animation/AnimationComponent.h"


/*
This class represents interface for accessing game engine functionalities;
GameObject class holds pointer to this interface;
Methods are overridden on the game engine side;
*/
class GameObjectInterface
{
public:
	virtual void SetScene(Yugo::Scene* scene) = 0;
	virtual Yugo::Scene* GetScene() = 0;

	virtual std::vector<entt::entity>& GetEntitiesWithEntityTagComponent() = 0;

	virtual TransformComponent& GetTransformComponent(Yugo::Entity& entity) = 0;
	virtual MeshComponent& GetMeshComponent(Yugo::Entity& entity) = 0;
	virtual AnimationComponent& GetAnimationComponent(Yugo::Entity& entity) = 0;
	virtual EntityTagComponent& GetEntityTagComponent(Yugo::Entity& entity) = 0;

	virtual bool HasTransformComponent(Yugo::Entity& entity) = 0;
	virtual bool HasMeshComponent(Yugo::Entity& entity) = 0;
	virtual bool HasAnimationComponent(Yugo::Entity& entity) = 0;
};


