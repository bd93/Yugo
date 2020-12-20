#pragma once
#include "Events/Event.h"
#include "Core/Core.h"
#include "Camera/Camera.h"

#include <entt/entt.hpp>

namespace Yugo
{

	class Scene;
	class Widget;

	class UserInterface
	{
		friend class Widget;
		friend class Editor; // World editor UI

		using TraverseFun = std::function<void(entt::entity)>;

	public:
		UserInterface();

		void OnStart();
		void OnEvent(const Event& event);
		void OnUpdate(TimeStep ts);
		void OnRender();
		void OnShutdown();

		void SaveUserInerface();
		void LoadUserInterface();

		Widget CreateWidget(const std::string& name = "");
		
		CameraComponent& GetCamera();

	private:
		entt::registry m_Registry;

	};

}