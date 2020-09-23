#pragma once
#include "Events/Event.h"
#include "Scene/Scene.h"
#include "Core/Dispatcher.h"
#include "WidgetComponent.h"


namespace Yugo
{

	class Widget;

	class UserInterface
	{
		friend class Widget;
		friend class Editor; // World editor UI

	public:
		UserInterface(sPtr<Scene>& scene);

		void OnStart();
		void OnEvent(const Event& event);
		void OnUpdate(float ts);
		void OnRender();
		void OnShutdown();

		void SaveUserInerface();
		void LoadUserInterface();

		Widget CreateWidget(const std::string& name = "");

	private:
		sPtr<Scene> m_Scene; // TEMPORARY!
		sPtr<Camera> m_Camera;
		entt::registry m_Registry;
	};

}