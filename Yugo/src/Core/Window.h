#pragma once
#include "Common.h"
#include "Events/KeyboardKeyEvent.h"
#include "Events/MouseButtonEvent.h"
#include "Events/ApplicationEvent.h"
#include "Dispatcher.h"
#include "Renderer/Renderer2D.h"
#include "ResourceManager.h"

#include <entt/entt.hpp>
#include <string>

namespace Yugo
{

	class Scene;

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
		void CreateGLFWwindow(GLFWmonitor* monitor, GLFWwindow* share);
		void ShowWindow();

		int const GetWindowWidth();
		int const GetWindowHeight();
		std::pair<int, int> GetWindowSize();

		static void InitGLFW();
		static void Hint(int hint, int value);
		static void PollEvents();
		static void MakeContextCurrent(GLFWwindow* glfwWindow);
		static void TerminateGLFW();
		
		glm::vec4 m_Color;

	private:
		sPtr<Scene> m_Scene;
		sPtr<UserInterface> m_UserInterface;

		GLFWwindow* m_GLFWwindow;
		std::string m_WindowName;

		int m_Width;
		int m_Height;
		
		void SetEventCallbacks();
		void RemoveEventCallbacks();
	};

}