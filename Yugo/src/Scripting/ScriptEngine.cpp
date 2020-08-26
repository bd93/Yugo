#include "pch.h"

#include "ScriptEngine.h"
#include "ScriptBehaviour.h"


namespace Yugo
{

	static std::unordered_map<std::string, Entity*> s_ScriptEntityMap;

	void Yugo::ScriptEngine::OnStart()
	{
		m_Lib = LoadLibrary(L"GameLogic.dll");
		if (m_Lib == NULL)
			std::cout << "Cannot dynamically load GameLogic.dll!\n";

		CreateScripts NewScripts = (CreateScripts)GetProcAddress(m_Lib, "CreateScripts");
		if (NewScripts == NULL)
			std::cout << "Cannot load CreateScripts function!\n";

		m_Scripts = NewScripts();

		for (auto script : m_Scripts)
		{
			ScriptBehaviour* scriptBehaviour = new ScriptBehaviour();
			auto& entity = s_ScriptEntityMap[script->GetScriptFilePath()];
			scriptBehaviour->SetEntity(entity);
			script->SetBehaviour(scriptBehaviour);
		}
	}

	void Yugo::ScriptEngine::OnUpdate(float ts)
	{
		for (auto script : m_Scripts)
			script->OnUpdate(ts);
	}

	void Yugo::ScriptEngine::OnShutdown()
	{

		// TODO: Implement all cases in order to fix a bug!
		
		DeleteScripts DestroyScripts = (DeleteScripts)GetProcAddress(m_Lib, "DeleteScripts");
		if (DestroyScripts == NULL)
			std::cout << "Cannot load DeleteScripts function!\n";
		
		DestroyScripts();

		FreeLibrary(m_Lib);
	}

	void ScriptEngine::OnStop()
	{
		FreeLibrary(m_Lib);
	}

	void ScriptEngine::OnReload()
	{
		m_Lib = LoadLibrary(L"GameLogic.dll");
		if (m_Lib == NULL)
			std::cout << "Cannot dynamically load GameLogic.dll!\n";
	}

	void ScriptEngine::AttachScript(const std::string& scriptFilePath, Entity* entity)
	{
		s_ScriptEntityMap[scriptFilePath] = entity;
	}

}
