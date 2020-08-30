#include "pch.h"
#include "Entity.h"




namespace Yugo
{

	Entity::Entity(entt::entity entityHandle, const std::string& name, Scene* scene)
		: m_Name(name), m_EntityHandle(entityHandle), m_Scene(scene)
	{
	}

	const std::string& Entity::GetName() const
	{
		return m_Name;
	}

	const uint32_t Entity::GetEntityId() const
	{
		return static_cast<uint32_t>(m_EntityHandle);
	}

	Scene* Entity::GetScene()
	{
		return m_Scene;
	}

}