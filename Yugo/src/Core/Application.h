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
		uPtr<Window> m_Window;
		uPtr<Editor> m_Editor;
		//uPtr<ScriptEngine> m_ScriptEngine;
		
		void OnStart();
		void OnShutdown();
	
		void InitOpenGL();
	};


}