#include "pch.h"
#include "Window.h"

#include <iostream>


namespace Yugo
{

	//GLFWwindow* Window::s_GLFWwindow = NULL;
	
	Window::Window(const std::string& windowName, int width, int height)
		: m_GLFWwindow(nullptr),
		m_WindowName(windowName),
		m_Width(width),
		m_Height(height)
	{
#ifdef YU_RELEASE
		m_Scene = sPtrCreate<Scene>();
#endif
	}

	void Window::OnStart()
	{
		SetEventCallbacks();
	}

	void Window::OnUpdate(TimeStep ts)
	{
	}

	void Window::OnEvent()
	{
	}

	void Window::OnRender()
	{
		//Renderer2D::ClearColorBuffer(0.2f, 0.2f, 0.2f);

		//Renderer2D::Draw(
		//	Shape2D::Line, 
		//	glm::vec3(100.0f, 0.0f, 0.0f), 
		//	glm::vec3(400.0f, 400.0f, 400.0f)
		//);
		//Renderer2D::Draw(
		//	Shape2D::Quad, 
		//	glm::vec3(50.0f, 0.0f, 0.0f), 
		//	glm::vec3(100.0f, 100.0f, 100.0f), 
		//	glm::vec3(1.0f, 1.0f, 0.2f)
		//);
		//Renderer2D::DrawRotated(
		//	Shape2D::Quad,
		//	glm::vec3(200.0f, 0.0f, 0.0f),
		//	glm::vec3(100.0f, 100.0f, 100.0f),
		//	20.0f,
		//	ResourceManager::GetTexture("awesomeface")
		//);
		//Renderer2D::DrawEmptyQuad(glm::vec3(-100.0f, 100.0f, 0.0f), 50);
		//Renderer2D::DrawGrid(glm::vec3(-100.0f, 100.0f, 0.0f), 16, 50);
	}

	void Window::OnShutdown()
	{
		glfwDestroyWindow(m_GLFWwindow);
	}

	bool Window::WindowShouldClose()
	{
		return glfwWindowShouldClose(m_GLFWwindow);
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_GLFWwindow);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::MakeContextCurrent(GLFWwindow* glfwWindow)
	{
		glfwMakeContextCurrent(glfwWindow);
	}

	void Window::TerminateGLFW()
	{
		glfwTerminate();
	}

	void Window::CreateGLFWwindow()
	{
		m_GLFWwindow = glfwCreateWindow(m_Width, m_Height, m_WindowName.c_str(), NULL, NULL);

		if (m_GLFWwindow == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
	}

	void Window::SetEventCallbacks()
	{
		glfwSetFramebufferSizeCallback(m_GLFWwindow, [](GLFWwindow* window, int width, int height)
			{
				glViewport(0, 0, width, height);
				WindowResize resizeEvent{ width, height };
				Dispatcher::Publish<WindowResize>(resizeEvent);
			}
		);

		glfwSetKeyCallback(m_GLFWwindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				if (action == GLFW_PRESS)
				{
					KeyboardKeyPress pressEvent{ static_cast<KeyCode>(key), 1 };
					Dispatcher::Publish<KeyboardKeyPress>(pressEvent);
				}
				if (action == GLFW_RELEASE)
				{
					KeyboardKeyRelease releaseEvent{ static_cast<KeyCode>(key) };
					Dispatcher::Publish<KeyboardKeyRelease>(releaseEvent);
				}
			}
		);

		glfwSetMouseButtonCallback(m_GLFWwindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				if (action == GLFW_PRESS)
				{
					MouseButtonPress clickEvent{ static_cast<MouseCode>(button), 1 };
					Dispatcher::Publish<MouseButtonPress>(clickEvent);
				}
				if (action == GLFW_RELEASE)
				{
					MouseButtonRelease releaseEvent{ static_cast<MouseCode>(button) };
					Dispatcher::Publish<MouseButtonRelease>(releaseEvent);
				}
			}
		);

		glfwSetScrollCallback(m_GLFWwindow, [](GLFWwindow* window, double offsetX, double offsetY) 
			{
				MouseScroll scrollEvent{ (float)offsetX, (float)offsetY };
				Dispatcher::Publish<MouseScroll>(scrollEvent);
			}
		);
	}

}