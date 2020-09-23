#include "pch.h"
#include "ScriptInterfaceImpl.h"
#include "Input/UserInput.h"
#include "Renderer/SpriteRenderer.h"

namespace Yugo
{

	bool ScriptInterfaceImpl::IsKeyboardkeyPressed(KeyCode key)
	{
		return UserInput::IsKeyboardKeyPressed(key);
	}

	bool ScriptInterfaceImpl::IsMouseButtonPressed(MouseCode button)
	{
		return UserInput::IsMouseButtonPressed(button);
	}

	bool ScriptInterfaceImpl::IsMouseHoveringRect(TransformComponent& transform)
	{
		//return UserInput::IsMouseHoveringRect(transform, mousePos);
		return MouseRay::CheckCollisionWithSprite(transform);
	}

	//glm::vec2 ScriptInterfaceImpl::GetMousePosition()
	//{
	//	//auto [mousePosX, mousePosY] = UserInput::GetMousePosition();
	//	//const auto& projection = SpriteRenderer::GetProjection();

	//	//// Transform mouse viewport coords to normalized device coords
	//	//float mouseNdcX = (2.0f * mousePosX) / 1600.0f - 1.0f; // TEMP!
	//	//float mouseNdcY = 1.0f - (2.0f * mousePosY) / 1200.0f; // TEMP!

	//	//// Transform normalized device coords to clip coords
	//	//glm::vec4 clipCoords = glm::vec4(mouseNdcX, mouseNdcY, -1.0f, 1.0f);

	//	//// Transform clip coords to eye space coords
	//	//glm::vec4 eyeCoords = glm::inverse(projection) * clipCoords;

	//	//return glm::vec2(eyeCoords.x, eyeCoords.y);
	//}

	Entity& ScriptInterfaceImpl::GetEntity()
	{
		return m_Entity;
	}

	void ScriptInterfaceImpl::SetEntity(Entity& entity)
	{
		m_Entity = entity;
	}

	TransformComponent& ScriptInterfaceImpl::GetTransformComponent()
	{
		return m_Entity.GetComponent<TransformComponent>();
	}

	MeshComponent& ScriptInterfaceImpl::GetMeshComponent()
	{
		return m_Entity.GetComponent<MeshComponent>();
	}

	AnimationComponent& ScriptInterfaceImpl::GetAnimationComponent()
	{
		return m_Entity.GetComponent<AnimationComponent>();
	}

	EntityTagComponent& ScriptInterfaceImpl::GetEntityTagComponent()
	{
		return m_Entity.GetComponent<EntityTagComponent>();
	}

	SpriteComponent& ScriptInterfaceImpl::GetSpriteComponent()
	{
		return m_Entity.GetComponent<SpriteComponent>();
	}

	bool ScriptInterfaceImpl::HasTransformComponent()
	{
		return m_Entity.HasComponent<TransformComponent>();
	}

	bool ScriptInterfaceImpl::HasMeshComponent()
	{
		return m_Entity.HasComponent<MeshComponent>();
	}

	bool ScriptInterfaceImpl::HasAnimationComponent()
	{
		return m_Entity.HasComponent<AnimationComponent>();
	}


}
