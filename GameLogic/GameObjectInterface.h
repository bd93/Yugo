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

	virtual TransformComponent& GetTransformComponent(entt::entity entity) = 0;
	virtual MeshComponent& GetMeshComponent(entt::entity entity) = 0;
	virtual AnimationComponent& GetAnimationComponent(entt::entity entity) = 0;
	virtual EntityTagComponent& GetEntityTagComponent(entt::entity entity) = 0;

	virtual std::vector<CanvasWidgetComponent*>& GetCanvasWidgetComponentsInChildren(entt::entity entity) = 0;
	virtual std::vector<ButtonWidgetComponent*>& GetButtonWidgetComponentsInChildren(entt::entity entity) = 0;
	virtual std::vector<TextWidgetComponent*>& GetTextWidgetComponentsInChildren(entt::entity entity) = 0;
	virtual std::vector<TransformComponent*>& GetTransformComponentsInChildren(entt::entity entity) = 0;

	virtual bool HasTransformComponent(entt::entity entity) = 0;
	virtual bool HasMeshComponent(entt::entity entity) = 0;
	virtual bool HasAnimationComponent(entt::entity entity) = 0;
	virtual bool HasEntityTagComponent(entt::entity entity) = 0;
};


