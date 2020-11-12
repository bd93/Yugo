#pragma once
#include "Common.h"
#include "Events/KeyboardKeyEvent.h"
#include "Events/MouseButtonEvent.h"
#include "Events/ApplicationEvent.h"
#include "Dispatcher.h"
#include "Renderer/Renderer2D.h"
#include "ResourceManager.h"
#include "Scene/Scene.h"

#include <entt/entt.hpp>
#include <string>

namespace Yugo
{

	class Window
	{
	public:
		Window(const std::string& windowName, int width, int height);

		void OnStart();
		void OnUpdate(TimeStep ts);
		void OnEvent();
		void OnRender();
		void OnShutdown();

		bool WindowShouldClose();
		void SwapBuffers();
		void PollEvents();

		static GLFWwindow* GetGLFWwindow(); // TODO: Check if there is better solution

	private:
		static GLFWwindow* s_GLFWwindow;
		std::string m_WindowName;
		int m_Width;
		int m_Height;
		
		void InitGLFW();
		void SetEventCallbacks();
	};

}