#pragma once
#include "Events/Event.h"
#include "Scene/Scene.h"
#include "Core/Dispatcher.h"

namespace Yugo
{

	class UserInterface
	{
	public:
		UserInterface(Scene* scene);

		void OnStart();
		void OnEvent(const Event& event);
		void OnUpdate(float ts);
		void OnRender();
		void OnShutdown();

		void SaveUserInerface();
		void LoadUserInterface();

	private:
		Scene* m_Scene;
	};

}