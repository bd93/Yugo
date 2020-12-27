#include "pch.h"
#include "Script.h"


namespace GameLogic
{

	/**
	 * @brief Returns client scripts
	 *
	 * This method initializes static vector that holds Script pointers.
	 * Client script is pushed into a vector when .cpp file is compiled,
	 * because each client script creates ScriptRegister static object.
	 * This object registers client script through template deduction.
	 *
	 * @note Static vector is created inside method's body because of weird error regarding explicit dll linking.
	 */
	std::vector<Script*>& Script::GetClientScripts()
	{
		static std::vector<Script*> scripts;
		return scripts;
	}

	void Script::SetGameObject(GameObject* gameObject)
	{
		m_GameObject = *gameObject;
	}

	const std::string& Script::GetScriptFilePath()
	{
		return m_ScriptFilePath;
	}


	extern "C"
	{

		SCRIPT_API ScriptArray CreateScripts()
		{
			auto& clientScripts = Script::GetClientScripts();
			auto numOfClientScripts = clientScripts.size();
			Script** scripts = new Script* [numOfClientScripts];
			size_t i = 0;
			for (auto clientScript : clientScripts)
				scripts[i++] = clientScript;

			ScriptArray scriptArray;
			scriptArray.Scripts = scripts;
			scriptArray.Size = numOfClientScripts;
			return scriptArray;
		}

		SCRIPT_API void DeleteScripts(ScriptArray scriptArray)
		{
			auto scripts = scriptArray.Scripts;
			auto& clientScripts = Script::GetClientScripts();
			size_t i = 0;
			for (auto clientScript : clientScripts)
			{
				scripts[i] = nullptr;
				delete scripts[i++];
				delete clientScript;
			}
		}

		SCRIPT_API void SetGameObject(GameObject* gameObject, Script* script)
		{
			script->SetGameObject(gameObject);
		}

		SCRIPT_API const char* GetScriptFilePath(Script* script)
		{
			return script->GetScriptFilePath().c_str();
		}
	}

}

