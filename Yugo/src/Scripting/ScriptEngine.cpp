#include "pch.h"

#include "ScriptEngine.h"


namespace Yugo
{

	/**
	 * @brief Method to be called during application OnStart stage.
	 *
	 * This method calls dll exported functions and creates client scripts and game objects.
	 * Then It creates interface implementations and sets it in client scripts and game objects.
	 * It also pushes interfaces and game objects to vectors in order to destroy them when OnStop method is called. 
	 */
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
			GameLogic::GameObject* gameObject = NewGameObject(entity.GetEnttEntity(), gameObjectInterfaceImpl);
			m_ScriptArray.Scripts[i]->SetGameObject(gameObject);
			m_GameObjects.push_back(gameObject);
			m_GameObjectInterfaceImpls.push_back(gameObjectInterfaceImpl);
		}

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnStart();
	}

	/**
	 * @brief Method to be called during application OnUpdate stage.
	 *
	 * Update scripts each frame.
	 * 
	 * @param timeStep Duration of each frame.
	 */
	void Yugo::ScriptEngine::OnUpdate(float timeStep)
	{
		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnUpdate(timeStep);
	}

	/**
	 * @brief Method to be called during application OnEvent stage.
	 *
	 * Calls OnEvent in each client's script.
	 *
	 * @param event Base class for all types of events.
	 */
	void ScriptEngine::OnEvent(const Event& event)
	{
		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnEvent(event);
	}

	void Yugo::ScriptEngine::OnShutdown()
	{
		// TODO: Implement case when user close window without clicking on "play scene" and "stop scene" buttons
	}

	/**
	 * @brief Method to be called when scripts are not in running state.
	 *
	 * OnStop calls exported dll functions in order to destroy objects allocated on dll heap.
	 * It also clears vectors so they can be used for the next script running state. 
	 */
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
		// TODO: Consider if this method is necessary or not.
	}

	/**
	 * @brief Keeps track of script file path and which entity this script is attached to.
	 *
	 * This method is called when user clicks "Attach Script" in world editor.
	 * The main idea is to fetch entity in ScriptEngine::OnStart method and sets it in script interface and game object.
	 *
	 * @param scriptFilePath Path to script file.
	 * @param entity Custom entity which holds entt::entity, name and pointer to scene.
	 */
	void ScriptEngine::AttachScript(const std::string& scriptFilePath, Entity& entity)
	{
		if (m_ScriptEntityMap.find(scriptFilePath) == m_ScriptEntityMap.end())
			m_ScriptEntityMap[scriptFilePath] = entity;
	}

	/* @brief Scene setter */
	void ScriptEngine::SetScene(Scene* scene)
	{
		m_Scene = scene;
	}

}
