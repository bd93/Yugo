#include "pch.h"
#include "Application.h"
#include "Time.h"
#include "Scripting/ScriptEngine.h"

namespace Yugo
{

	void Application::OnStart()
	{
		m_Window->OnStart();
		InitOpenGL();
		m_Editor->OnStart();
		Renderer2D::Init();
		Time::OnStart();
		//m_ScriptEngine->OnStart();
	}

	void Application::OnShutdown()
	{
		m_Editor->OnShutdown();
		m_Window->OnShutdown();
	}

	void Application::InitOpenGL()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}
	}

	Application::Application()
	{
		m_Window = uPtrCreate<Window>("Yugo", 1200, 800);
		m_Editor = uPtrCreate<Editor>();
		//m_ScriptEngine = uPtrCreate<ScriptEngine>();
	}

	Application::~Application()
	{
		OnShutdown();
	}

	void Yugo::Application::Run()
	{
		OnStart();

		// Game loop
		while (!m_Window->WindowShouldClose())
		{
			m_Window->PollEvents();

			// Input
			m_Window->OnEvent();

			//// Scripts
			//m_ScriptEngine->OnUpdate(Time::GetDeltaTime());

			// Update
			m_Window->OnUpdate(Time::GetDeltaTime());
			m_Editor->OnUpdate(Time::GetDeltaTime());

			// Render
			//m_Window->OnRender();
			m_Editor->OnRender();

			// Swap buffers
			m_Window->SwapBuffers();

			// Calculate time of 1 frame
			Time::CalculateDeltaTime();
		}
	}

}
