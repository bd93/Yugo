#include "pch.h"
#include "Script.h"


#define SCRIPT_API __declspec(dllexport)


namespace GameLogic
{

	std::vector<Script*>& Script::GetClientScripts()
	{
		static std::vector<Script*> scripts;
		return scripts;
	}


	extern "C"
	{

		SCRIPT_API ScriptArray CreateScripts()
		{
			auto& clientScripts = Script::GetClientScripts();
			auto numOfClientScripts = clientScripts.size();
			Script** scripts = new Script * [numOfClientScripts];
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
	}

}

