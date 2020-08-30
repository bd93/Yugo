#include "pch.h"

#include "ScriptEngine.h"


namespace Yugo
{

	ScriptEngine::~ScriptEngine()
	{
		for (auto scriptEntityPair : m_ScriptEntityMap)
			delete scriptEntityPair.second;
	}

	void Yugo::ScriptEngine::OnStart()
	{
		m_Lib = LoadLibrary(L"GameLogic.dll");
		if (m_Lib == NULL)
			std::cout << "Cannot dynamically load GameLogic.dll!\n";

		CreateScripts NewScripts = (CreateScripts)GetProcAddress(m_Lib, "CreateScripts");
		if (NewScripts == NULL)
			std::cout << "Cannot load CreateScripts function!\n";

		m_ScriptArray = NewScripts();

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
		{
			m_ScriptInterfaceImpl = new ScriptInterfaceImpl();
			auto entity = m_ScriptEntityMap[m_ScriptArray.Scripts[i]->GetScriptFilePath()];
			m_ScriptInterfaceImpl->SetEntity(entity);
			m_ScriptArray.Scripts[i]->SetScriptInterface(m_ScriptInterfaceImpl);
		}

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnStart();
	}

	void Yugo::ScriptEngine::OnUpdate(float ts)
	{
		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnUpdate(ts);
	}

	void Yugo::ScriptEngine::OnShutdown()
	{
		// TODO: Implement case when user close window without clicking on play scene and stop scene buttons
	}

	void ScriptEngine::OnStop()
	{
		delete m_ScriptInterfaceImpl;

		DeleteScripts DestroyScripts = (DeleteScripts)GetProcAddress(m_Lib, "DeleteScripts");
		if (DestroyScripts == NULL)
			std::cout << "Cannot load DeleteScripts function!\n";
		DestroyScripts(m_ScriptArray);
		
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
		m_ScriptEntityMap[scriptFilePath] = entity;
	}

}
