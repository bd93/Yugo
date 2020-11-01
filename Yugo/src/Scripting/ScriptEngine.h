#pragma once
#include "Scene/Entity.h"
#include "Script.h"
#include "GameObject.h"
#include "ScriptInterfaceImpl.h"
#include "GameObjectInterfaceImpl.h"

#include <Windows.h>


namespace Yugo
{
	// Function pointers for imported functions from GameLogic.dll
	typedef GameLogic::ScriptArray (*CreateScripts)();
	typedef void (*DeleteScripts)(GameLogic::ScriptArray);
	typedef GameLogic::GameObject* (*CreateGameObject)(entt::entity, GameObjectInterfaceImpl*);
	typedef void (*DeleteGameObject)(GameLogic::GameObject*);
	
	class Editor;

	/**
	 * @brief Script engine for running c++ "scripts"
	 *
	 * The core idea is to "tie" GameLogic dll with Yugo game engine static library and to provide functionality to reload dll while engine is running.
	 * Because of that, GameLogic dll is linked explicitly via LoadLibrary and GetProcAddress windows api functions.
	 * In GameLogic dll interface classes define game engine functionalities on the GameLogic side.
	 * These functionalities are implemented on the game engine side by inheriting interfaces and overriding pure virtual methods.
	 * 
	 * Scripts and GameObjects, which scripts are attached to, are created on the GameLogic heap by calling exported functions on the game engine side.
	 * ScriptEngine class is responsible to call client script's methods OnStart, OnEvent, OnUpdate
	 *
	 * @note Be cautious where you are allocating objects on the heap, becuase you can't allocate object on the heap in GameLogic dll
	 * and free (destroy) it on the game engine side!
	 */
	class ScriptEngine
	{
		friend class Editor;

	public:
		void OnStart();
		void OnUpdate(float timeStep);
		void OnEvent(const Event& event);
		void OnShutdown();
		void OnStop();
		void OnReload();

		void AttachScript(const std::string& scriptFilePath, Entity& entity);
		void SetScene(Scene* scene);

	private:
		HINSTANCE m_Lib;
		GameLogic::ScriptArray m_ScriptArray;
		Scene* m_Scene;
		std::vector<GameLogic::GameObject*> m_GameObjects;
		std::vector<ScriptInterfaceImpl*> m_ScriptInterfaceImpls;
		std::vector<GameObjectInterfaceImpl*> m_GameObjectInterfaceImpls;
		std::unordered_map<std::string, Entity> m_ScriptEntityMap;
	};
}