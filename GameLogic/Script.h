#pragma once
#include "GameObject.h"
#include "YugoComponents.h"
#include "YugoEvents.h"
#include "Common.h"
#include "Input.h"


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

		virtual void OnStart() {};
		virtual void OnUpdate(TimeStep ts) {};

		// Events (messages)
		virtual void OnMouseLeftClick() {}
		virtual void OnMouseLeftRelease() {}
		virtual void OnMouseRightClick() {}
		virtual void OnMouseRightRelease() {}
		virtual void OnMouseOver() {}
		virtual void OnMouseExit() {}

		void SetGameObject(GameObject* gameObject);
		const std::string& GetScriptFilePath();
		static std::vector<Script*>& GetClientScripts();

	protected:
		GameObject m_GameObject; // Game object which this script is attached to
		std::string m_ScriptFilePath;
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

