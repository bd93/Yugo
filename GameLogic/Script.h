#pragma once
#include "GameObject.h"
#include "YugoComponents.h"
#include "YugoEvents.h"
#include "Macros.h"

namespace GameLogic
{

	/**
	 * @brief Base class for all client scripts.
	 *
	 * Methods are overriden on the game engine side in ScriptInterfaceImpl class.
	 * This class holds ScriptInterface pointer which points to ScriptInterfaceImpl.
	 * Methods call engine functionalities through the pointer.
	 */
	class Script
	{
	public:
		virtual ~Script() {}

		virtual void OnStart() = 0;
		virtual void OnUpdate(TimeStep ts) = 0;
		virtual void OnEvent(const Yugo::Event& event) = 0;
		virtual void OnShutdown() = 0;

		//void SetGameObject(GameObject* gameObject);
		const std::string& GetScriptFilePath();

		//static void SetInterface(ScriptInterface* scriptInterface);
		static std::vector<Script*>& GetClientScripts();

	protected:
		//GameObject* m_GameObject; // Game object which this script is attached to
		std::string m_ScriptFilePath;

	private:
		//static ScriptInterface* s_ScriptInterface;
	};

	/**
	 * @brief Client script registration
	 *
	 * Client script is registered when .cpp file is compiled,
	 * ScriptRegister object registers client script through template deduction.
	 */
	template<typename ClientScript>
	struct ScriptRegister
	{
		ScriptRegister()
		{
			ClientScript* clientScript = new ClientScript();
			Script::GetClientScripts().push_back(clientScript);
		}
	};

	/**
	 * @brief Array of client script pointers
	 *
	 * This struct holds array of Script pointers and it's size.
	 * Array of script pointers is needed in order to transform std::vector to classic dynamic array,
	 * so it can be exported through extern "C" dll function
	 * 
	 * @note Standard vector can't be exported via extern "C" dll function
	 */
	struct ScriptArray
	{
		size_t Size;
		Script** Scripts;
	};

}

