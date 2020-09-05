#include "pch.h"
#include "ScriptInterfaceImpl.h"
#include "Input/UserInput.h"

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
