#include "pch.h"

#include "ScriptEngine.h"

#include "EngineFuncs.h"


namespace Yugo
{

	ScriptEngine::ScriptEngine()
		: m_Scene(nullptr)
	{
	}

	/**
	 * @brief Method to be called during application OnStart stage.
	 *
	 * This method calls dll exported functions and creates client scripts and game objects.
	 * Then it creates interface implementations and sets it in client scripts and game objects.
	 * It also pushes interfaces and game objects to vectors in order to destroy them when OnStop method is called. 
	 */
	void Yugo::ScriptEngine::OnStart(Scene* scene)
	{
#ifdef YU_RELEASE
		Dispatcher::Subscribe<KeyboardKeyPress>(this);
		Dispatcher::Subscribe<MouseButtonPress>(this);
		Dispatcher::Subscribe<MouseScroll>(this);
		Dispatcher::Subscribe<ImportAssetEvent>(this);
#endif

		m_Scene = scene;

		m_Lib = LoadLibrary(L"GameLogic.dll");
		if (m_Lib == NULL)
			std::cout << "Cannot dynamically load GameLogic.dll!\n";

		CreateScripts NewScripts = (CreateScripts)GetProcAddress(m_Lib, "CreateScripts");
		if (NewScripts == NULL)
			std::cout << "Cannot load CreateScripts function!\n";
		CreateGameObject NewGameObject = (CreateGameObject)GetProcAddress(m_Lib, "CreateGameObject");
		if (NewGameObject == NULL)
			std::cout << "Cannot load CreateGameObject function!\n";
		GetScriptFilePath ScriptFilePath = (GetScriptFilePath)GetProcAddress(m_Lib, "GetScriptFilePath");
		if (ScriptFilePath == NULL)
			std::cout << "Cannot load GetScriptFilePath function!\n";
		SetGameObject SetScriptGameObject = (SetGameObject)GetProcAddress(m_Lib, "SetGameObject");
		if (SetScriptGameObject == NULL)
			std::cout << "Cannot load SetGameObject function!\n";
		ImportGameEngineFuncs SetGameEngineFuncs = (ImportGameEngineFuncs)GetProcAddress(m_Lib, "ImportGameEngineFuncs");
		if (SetGameEngineFuncs == NULL)
			std::cout << "Cannot load ImportGameEngineFuncs function!\n";

		GameLogic::GameEngineFuncs gameEngineFuncs;
		gameEngineFuncs.GetTransformComponent = std::bind(&Scene::template GetComponent<TransformComponent>, m_Scene, std::placeholders::_1);
		SetGameEngineFuncs(gameEngineFuncs);

		m_ScriptArray = NewScripts();

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
		{
			//Entity& entity = m_ScriptEntityMap[ScriptFilePath(m_ScriptArray.Scripts[i])];
			auto entity = m_ScriptEntityMap[ScriptFilePath(m_ScriptArray.Scripts[i])];
			auto& scriptComponent = m_Scene->GetComponent<ScriptComponent>(entity);
			scriptComponent.ClientScript = m_ScriptArray.Scripts[i];

			GameLogic::GameObject* gameObject = NewGameObject(entity);
			m_GameObjects.push_back(gameObject);
			SetScriptGameObject(gameObject, m_ScriptArray.Scripts[i]);
		}

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnStart();
	}

	/* @brief Update scripts each frame. */
	void Yugo::ScriptEngine::OnUpdate(TimeStep ts)
	{
		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnUpdate(ts);
	}

	/* @brief Calls OnEvent each frame in each client's script. */
	void ScriptEngine::OnEvent(const Event& event)
	{
		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnEvent(event);
	}

	/* @brief Calls OnShutdown each frame in each client's script. */
	void Yugo::ScriptEngine::OnShutdown()
	{
		// TODO: Implement case when user close window without clicking on "play scene" and "stop scene" buttons
	}

	/**
	 * @brief Method to be called when script is stopped.
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
			DestroyGameObject(m_GameObjects[i]);
		}

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
	void ScriptEngine::AttachScript(const std::string& scriptFilePath, entt::entity entity)
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
