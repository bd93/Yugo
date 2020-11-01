#include "pch.h"
#include "Widget.h"



namespace Yugo
{
	Widget::Widget(entt::entity entityHandle, const std::string& name, UserInterface* userInterface)
		: m_EntityHandle(entityHandle), 
		m_Name(name), 
		m_UserInterface(userInterface)
	{
	}

	const std::string& Widget::GetName() const
	{
		return m_Name;
	}

	const entt::entity Widget::GetEnttEntity() const
	{
		return m_EntityHandle;
	}
}