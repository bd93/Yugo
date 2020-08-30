#pragma once
#include "Scene/Entity.h"
#include "Script.h"
#include "ScriptInterfaceImpl.h"

#include <Windows.h>


namespace Yugo
{
	class Editor;

	typedef ScriptArray (*CreateScripts)();
	typedef void (*DeleteScripts)(ScriptArray);

	class ScriptEngine
	{
		friend class Editor;

	public:
		~ScriptEngine();

		void OnStart();
		void OnUpdate(float ts);
		void OnShutdown();
		void OnStop();
		void OnReload();

		void AttachScript(const std::string& scriptFilePath, Entity* entity);

	private:
		HINSTANCE m_Lib;
		ScriptArray m_ScriptArray;
		ScriptInterfaceImpl* m_ScriptInterfaceImpl;
		std::unordered_map<std::string, Entity*> m_ScriptEntityMap;
	};
}