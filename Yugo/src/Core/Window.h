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
		friend class Editor;
		friend class Application;

	public:
		Window(const std::string& windowName, int width, int height);

		void OnStart();
		void OnUpdate(TimeStep ts);
		void OnEvent();
		void OnRender();
		void OnShutdown();

		bool WindowShouldClose();
		void SwapBuffers();
		void CreateGLFWwindow();

		static void PollEvents();
		static void MakeContextCurrent(GLFWwindow* glfwWindow);
		static void TerminateGLFW();

	private:
#ifdef YU_RELEASE
		sPtr<Scene> m_Scene;
#endif
		GLFWwindow* m_GLFWwindow;
		std::string m_WindowName;
		int m_Width;
		int m_Height;
		
		void SetEventCallbacks();
	};

}