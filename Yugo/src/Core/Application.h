#pragma once
#include "Common.h"
#include "Core.h"
#include "Window.h"
#include "Editor/Editor.h"
#include "Scripting/ScriptEngine.h"

#include <iostream>


namespace Yugo
{

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

	private:

#ifdef YU_RELEASE
		uPtr<Window> m_Window;
		uPtr<ScriptEngine> m_ScriptEngine
#endif
#ifdef YU_DEBUG
		uPtr<Editor> m_Editor;
#endif
		
		void OnStart();
		void OnShutdown();
	
		void InitOpenGL();
		void InitGLFW();
	};


}