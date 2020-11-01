#pragma once
#include "Scene/Entity.h"
#include "Animation/Components.h"
#include "Scene/Components.h"
#include "Components.h"


namespace GameLogic
{

	/**
	 * @brief Script interface for accessing game engine functionalities.
	 *
	 * Methods are overriden on the game engine side in ScriptInterfaceImpl class.
	 * Each client's script holds ScriptInterface pointer which points to ScriptInterfaceImpl.
	 * Script class, which is a base class for client script, defines wrapper methods that calls interface methods.
	 */
	class ScriptInterface
	{
	public:
		virtual bool IsKeyboardkeyPressed(Yugo::KeyCode key) = 0;
		virtual bool IsMouseButtonPressed(Yugo::MouseCode button) = 0;
		virtual bool IsMouseHoveringRect(TransformComponent& transform) = 0;
		//virtual glm::vec2 GetMousePosition() = 0;

		virtual Yugo::Entity& GetEntity() = 0;
		virtual void SetEntity(Yugo::Entity& entity) = 0;

		virtual TransformComponent& GetTransformComponent() = 0;
		virtual MeshComponent& GetMeshComponent() = 0;
		virtual AnimationComponent& GetAnimationComponent() = 0;
		virtual EntityTagComponent& GetEntityTagComponent() = 0;
		virtual SpriteComponent& GetSpriteComponent() = 0;

		virtual bool HasTransformComponent() = 0;
		virtual bool HasMeshComponent() = 0;
		virtual bool HasAnimationComponent() = 0;
		virtual bool HasEntityTagComponent() = 0;
		virtual bool HasSpriteComponent() = 0;
	};

}
