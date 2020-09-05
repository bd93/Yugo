#pragma once
#include "Scene/Entity.h"
#include "Script.h"
#include "GameObject.h"
#include "ScriptInterfaceImpl.h"
#include "GameObjectInterfaceImpl.h"

#include <Windows.h>


namespace Yugo
{
	class Editor;

	typedef ScriptArray (*CreateScripts)();
	typedef void (*DeleteScripts)(ScriptArray);
	typedef GameObject* (*CreateGameObject)(GameObjectInterfaceImpl*);
	typedef void (*DeleteGameObject)(GameObject*);

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

		void AttachScript(const std::string& scriptFilePath, Entity& entity);
		void SetScene(Scene* scene);

	private:
		HINSTANCE m_Lib;
		ScriptArray m_ScriptArray;
		Scene* m_Scene;
		std::vector<GameObject*> m_GameObjects;
		std::vector<ScriptInterfaceImpl*> m_ScriptInterfaceImpls;
		std::vector<GameObjectInterfaceImpl*> m_GameObjectInterfaceImpls;
		std::unordered_map<std::string, Entity> m_ScriptEntityMap;
	};
}