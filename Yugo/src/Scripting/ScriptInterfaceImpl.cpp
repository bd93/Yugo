#include "pch.h"
#include "ScriptInterfaceImpl.h"
#include "Input/UserInput.h"

namespace Yugo
{

	bool ScriptInterfaceImpl::IsKeyboardkeyPressed(Yugo::KeyCode key)
	{
		return UserInput::IsKeyboardKeyPressed(key);
	}

	bool ScriptInterfaceImpl::IsMouseButtonPressed(Yugo::MouseCode button)
	{
		return UserInput::IsMouseButtonPressed(button);
	}

	Yugo::Entity* ScriptInterfaceImpl::GetEntity()
	{
		return m_Entity;
	}

	void ScriptInterfaceImpl::SetEntity(Yugo::Entity* entity)
	{
		m_Entity = entity;
	}

	Yugo::TransformComponent& ScriptInterfaceImpl::GetTransformComponent()
	{
		return m_Entity->GetComponent<Yugo::TransformComponent>();
	}

	Yugo::MeshComponent& ScriptInterfaceImpl::GetMeshComponent()
	{
		return m_Entity->GetComponent<Yugo::MeshComponent>();
	}

	Yugo::AnimationComponent& ScriptInterfaceImpl::GetAnimationComponent()
	{
		return m_Entity->GetComponent<Yugo::AnimationComponent>();
	}

	bool ScriptInterfaceImpl::HasTransformComponent()
	{
		return m_Entity->HasComponent<Yugo::TransformComponent>();
	}

	bool ScriptInterfaceImpl::HasMeshComponent()
	{
		return m_Entity->HasComponent<Yugo::MeshComponent>();
	}

	bool ScriptInterfaceImpl::HasAnimationComponent()
	{
		return m_Entity->HasComponent<Yugo::AnimationComponent>();
	}

}
