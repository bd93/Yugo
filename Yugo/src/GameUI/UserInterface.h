#pragma once
#include "Events/Event.h"
#include "Core/Core.h"

#include <entt/entt.hpp>

namespace Yugo
{

	class Scene;
	class Widget;

	class UserInterface
	{
		friend class Widget;
		friend class Editor; // World editor UI

	public:
		UserInterface(Scene* scene);

		void OnStart();
		void OnEvent(const Event& event);
		void OnUpdate(TimeStep ts);
		void OnRender();
		void OnShutdown();

		void SaveUserInerface();
		void LoadUserInterface();

		Widget CreateWidget(const std::string& name = "");

	private:
		Scene* m_Scene;
		//sPtr<Camera> m_Camera;
		entt::registry m_Registry;
	};

}