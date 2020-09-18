#pragma once
#include "Scene.h"



namespace Yugo
{

	class Entity
	{
	public:
		Entity(entt::entity entityHandle, const std::string& name, Scene* scene);
		Entity() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);
		template<typename T>
		T& GetComponent();
		template<typename T>
		bool HasComponent();
		template<typename T>
		void RemoveComponent();

		const std::string& GetName() const;
		const entt::entity GetEnttEntity() const;
		Scene* GetScene(); // TEMPORARY!!!

	private:
		entt::entity m_EntityHandle;
		std::string m_Name;
		Scene* m_Scene;
	};

	template<typename T, typename ...Args>
	inline T& Entity::AddComponent(Args&& ...args)
	{
		assert(!HasComponent<T>());
		return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	inline T& Entity::GetComponent()
	{
		assert(HasComponent<T>());
		return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	inline bool Entity::HasComponent()
	{
		return m_Scene->m_Registry.has<T>(m_EntityHandle);
	}

	template<typename T>
	inline void Entity::RemoveComponent()
	{
		assert(HasComponent<T>());
		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

}