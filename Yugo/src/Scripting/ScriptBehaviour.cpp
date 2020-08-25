#include "pch.h"
#include "ScriptBehaviour.h"
#include "Input/UserInput.h"

namespace Yugo
{

	bool ScriptBehaviour::IsKeyboardkeyPressed(Yugo::KeyCode key)
	{
		return UserInput::IsKeyboardKeyPressed(key);
	}

	bool ScriptBehaviour::IsMouseButtonPressed(Yugo::MouseCode button)
	{
		return UserInput::IsMouseButtonPressed(button);
	}

	Yugo::Entity* ScriptBehaviour::GetEntity()
	{
		return m_Entity;
	}

	void ScriptBehaviour::SetEntity(Yugo::Entity* entity)
	{
		m_Entity = entity;
	}

	Yugo::TransformComponent& ScriptBehaviour::GetTransformComponent()
	{
		return m_Entity->GetComponent<Yugo::TransformComponent>();
	}

	Yugo::MeshComponent& ScriptBehaviour::GetMeshComponent()
	{
		return m_Entity->GetComponent<Yugo::MeshComponent>();
	}

	Yugo::AnimationComponent& ScriptBehaviour::GetAnimationComponent()
	{
		return m_Entity->GetComponent<Yugo::AnimationComponent>();
	}

	bool ScriptBehaviour::HasTransformComponent()
	{
		return m_Entity->HasComponent<Yugo::TransformComponent>();
	}

	bool ScriptBehaviour::HasMeshComponent()
	{
		return m_Entity->HasComponent<Yugo::MeshComponent>();
	}

	bool ScriptBehaviour::HasAnimationComponent()
	{
		return m_Entity->HasComponent<Yugo::AnimationComponent>();
	}

}
