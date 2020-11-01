#pragma once
#include "UserInterface.h"



namespace Yugo
{

	class Widget
	{
	public:
		Widget(entt::entity entityHandle, const std::string& name, UserInterface* userInterface);
		Widget() = default;
		Widget(const Widget& other) = default;

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

	private:
		entt::entity m_EntityHandle;
		std::string m_Name;
		UserInterface* m_UserInterface;
	};

	template<typename T, typename ...Args>
	inline T& Widget::AddComponent(Args&& ...args)
	{
		assert(!HasComponent<T>());
		return m_UserInterface->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	inline T& Widget::GetComponent()
	{
		assert(HasComponent<T>());
		return m_UserInterface->m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	inline bool Widget::HasComponent()
	{
		return m_UserInterface->m_Registry.has<T>(m_EntityHandle);
	}

	template<typename T>
	inline void Widget::RemoveComponent()
	{
		assert(HasComponent<T>());
		m_UserInterface->m_Registry.remove<T>(m_EntityHandle);
	}

}