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
		uPtr<Window> m_SecondWindow;
		uPtr<Editor> m_Editor;
		//ScriptEngine* m_ScriptEngine;
		//Scene* m_Scene;
		
		void OnStart();
		void OnShutdown();
	
		void InitOpenGL();
	};


}