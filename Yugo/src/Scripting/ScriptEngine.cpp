#include "pch.h"

#include "ScriptEngine.h"


namespace Yugo
{

	ScriptEngine::~ScriptEngine()
	{
		//for (auto scriptEntityPair : m_ScriptEntityMap)
		//	delete scriptEntityPair.second;
	}

	void Yugo::ScriptEngine::OnStart()
	{
		m_Lib = LoadLibrary(L"GameLogic.dll");
		if (m_Lib == NULL)
			std::cout << "Cannot dynamically load GameLogic.dll!\n";

		CreateScripts NewScripts = (CreateScripts)GetProcAddress(m_Lib, "CreateScripts");
		if (NewScripts == NULL)
			std::cout << "Cannot load CreateScripts function!\n";
		CreateGameObject NewGameObject = (CreateGameObject)GetProcAddress(m_Lib, "CreateGameObject");
		if (NewGameObject == NULL)
			std::cout << "Cannot load CreateGameObject function!\n";

		m_ScriptArray = NewScripts();

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
		{
			auto entity = m_ScriptEntityMap[m_ScriptArray.Scripts[i]->GetScriptFilePath()];
			
			ScriptInterfaceImpl* scriptInterfaceImpl = new ScriptInterfaceImpl();
			scriptInterfaceImpl->SetEntity(entity);
			m_ScriptArray.Scripts[i]->SetScriptInterface(scriptInterfaceImpl);
			m_ScriptInterfaceImpls.push_back(scriptInterfaceImpl);

			GameObjectInterfaceImpl* gameObjectInterfaceImpl = new GameObjectInterfaceImpl();
			gameObjectInterfaceImpl->SetScene(m_Scene);
			::GameObject* gameObject = NewGameObject(entity.GetEnttEntity(), gameObjectInterfaceImpl);
			m_ScriptArray.Scripts[i]->SetGameObject(gameObject);
			m_GameObjects.push_back(gameObject);
			m_GameObjectInterfaceImpls.push_back(gameObjectInterfaceImpl);
		}

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnStart();
	}

	void Yugo::ScriptEngine::OnUpdate(float ts)
	{
		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnUpdate(ts);
	}

	void ScriptEngine::OnEvent(const Event& event)
	{
		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnEvent(event);
	}

	void Yugo::ScriptEngine::OnShutdown()
	{
		// TODO: Implement case when user close window without clicking on play scene and stop scene buttons
	}

	void ScriptEngine::OnStop()
	{
		DeleteGameObject DestroyGameObject = (DeleteGameObject)GetProcAddress(m_Lib, "DeleteGameObject");
		if (DestroyGameObject == NULL)
			std::cout << "Cannot load DeleteGameObject function!\n";

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
		{
			delete m_GameObjectInterfaceImpls[i];
			delete m_ScriptInterfaceImpls[i];
			DestroyGameObject(m_GameObjects[i]);
		}

		m_GameObjectInterfaceImpls.clear();
		m_ScriptInterfaceImpls.clear();
		m_GameObjects.clear();

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

	void ScriptEngine::AttachScript(const std::string& scriptFilePath, Entity& entity)
	{
		m_ScriptEntityMap[scriptFilePath] = entity;
	}

	void ScriptEngine::SetScene(Scene* scene)
	{
		m_Scene = scene;
	}

}
