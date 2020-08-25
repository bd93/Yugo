#pragma once
#include "Scene/Entity.h"
#include "Script.h"

#include <Windows.h>


namespace Yugo
{
	class Editor;

	typedef std::vector<Script*> (*CreateScripts)();
	typedef void (*DeleteScripts)();

	class ScriptEngine
	{
		friend class Editor;

	public:
		void OnStart();
		void OnUpdate(float ts);
		void OnShutdown();
		void OnStop();
		void OnReload();

		void AttachScript(const std::string& scriptFilePath, Entity* entity);

	private:
		HINSTANCE m_Lib;
		std::vector<Script*> m_Scripts;
	};
}