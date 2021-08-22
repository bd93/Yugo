#pragma once
#include "Scene/Entity.h"
#include "Script.h"
#include "Components.h"
#include "GameObject.h"
#include "EngineFuncs.h"
#include "Core/Window.h"
//#include "ScriptInterfaceImpl.h"
//#include "GameObjectInterfaceImpl.h"
//#include "PlaneInterfaceImpl.h"
//#include "InputInterfaceImpl.h"
//#include "MouseRayInterfaceImpl.h"


#include <Windows.h>


namespace Yugo
{
	// Function pointers which point to functions, imported from GameLogic.dll
	typedef GameLogic::ScriptArray (*CreateScripts)();
	typedef GameLogic::GameObject* (*CreateGameObject)(entt::entity);
	typedef void (*DeleteScripts)(GameLogic::ScriptArray);
	typedef void (*DeleteGameObject)(GameLogic::GameObject*);
	typedef const char* (*GetScriptFilePath)(GameLogic::Script* script);
	typedef void (*SetGameObject)(GameLogic::GameObject*, GameLogic::Script* script);
	typedef void (*ImportGameEngineFuncs)(GameLogic::GameEngineFuncs);
	typedef void (*ImportUiEngineFuncs)(GameLogic::UiEngineFuncs);
	
	class Editor;

	/**
	 * @brief Script engine for running c++ "scripts"
	 *
	 * The core idea is to "tie" GameLogic dll with Yugo game engine static library and to provide functionality to reload dll while engine is running.
	 * Because of that, GameLogic dll is linked explicitly via LoadLibrary and GetProcAddress windows api functions.
	 * GameEngineFuncs class defines game engine functionalities on the GameLogic side (std::function type).
	 * These functionalities are implemented on the game engine side and ScriptEngine assaign these functionalities to dll GameEngineFuncs std::function type members.
	 * 
	 * Scripts and GameObjects, which scripts are attached to, are created on the GameLogic heap by calling dll exported functions on the game engine side.
	 * ScriptEngine class calls client script's virtual methods OnStart and OnUpdate each frame.
	 *
	 * @note Be cautious where you are allocating objects on the heap, becuase you can't allocate object on the heap in GameLogic dll
	 * and free (destroy) it on the game engine side!
	 */
	class ScriptEngine
	{
		friend class Editor;

	public:

		void OnStart(Scene* scene, UserInterface* ui, Window* window);
		void OnUpdate(TimeStep ts);
		void OnEvent(const Event& event);
		void OnShutdown();
		void OnStop();
		void OnReload();

		void AttachScript(const std::string& scriptFilePath, entt::entity entity);
		void SetScene(Scene* scene);
		void SetUserInterface(UserInterface* ui);

	private:
		HINSTANCE m_Lib;
		GameLogic::ScriptArray m_ScriptArray;
		Scene* m_Scene = nullptr;
		UserInterface* m_UserInterface = nullptr;
		Window* m_Window;
		std::vector<GameLogic::GameObject*> m_GameObjects;
		std::unordered_map<std::string, entt::entity> m_ScriptEntityMap;

		void BindGameEngineFunctionalities(GameLogic::GameEngineFuncs& gameEngineFuncs);
		void BindUiEngineFunctionalities(GameLogic::UiEngineFuncs& uiEngineFuncs);
	};
}