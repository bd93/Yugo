#include "pch.h"
#include "Application.h"
#include "Time.h"
#include "Scripting/ScriptEngine.h"

namespace Yugo
{

	Application::Application()
	{
#ifdef YU_DEBUG
		m_Editor = uPtrCreate<Editor>();
#endif
#ifdef YU_RELEASE
		m_Window = uPtrCreate<Window>();
		m_ScriptEngine = uPtrCreate<ScriptEngine>();
#endif
	}

	Application::~Application()
	{
		OnShutdown();
	}

	void Application::OnStart()
	{
		Window::InitGLFW();
		m_Editor->m_MainWindow->CreateGLFWwindow(NULL, NULL);
		Window::MakeContextCurrent(m_Editor->m_MainWindow->m_GLFWwindow);
		InitOpenGL();

#ifdef YU_DEBUG
		m_Editor->OnStart();
#endif
#ifdef YU_RELEASE
		m_Window->CreateGLFWwindow();
		UserInput::SetGLFWwindow(m_Window->m_GLFWwindow);
		m_ScriptEngine->SetScene(m_Window->m_Scene.get());
		// TODO: set script engine OnStart, OnStop, AttachScript methods!
#endif
		//Renderer2D::OnStart();
		Time::OnStart();
	}

	void Application::OnShutdown()
	{
#ifdef YU_DEBUG
		m_Editor->OnShutdown();
#endif
#ifdef YU_RELEASE
		m_ScriptEngine->OnShutdown();
#endif
	}

	void Application::InitOpenGL()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}
	}

	void Yugo::Application::Run()
	{
		OnStart();

		// Game loop
		while (!m_Editor->m_MainWindow->WindowShouldClose())
		{
			//Window::PollEvents();

			// Update
#ifdef YU_DEBUG
			m_Editor->OnUpdate(Time::GetDeltaTime());
#endif
#ifdef YU_RELEASE
			m_ScriptEngine->OnUpdate(Time::GetDeltaTime());
			m_Window->OnUpdate(Time::GetDeltaTime());
#endif

			// Render
#ifdef YU_DEBUG
			m_Editor->OnRender();
#endif
#ifdef YU_RELEASE
			m_Window->OnRender();
#endif

			Window::PollEvents();
			// Swap buffers
#ifdef YU_DEBUG
			m_Editor->m_MainWindow->SwapBuffers();
#endif
#ifdef YU_RELEASE
			m_Window->SwapBuffers();
#endif

			// Calculate time of 1 frame
			Time::CalculateDeltaTime();
		}
	}

}
