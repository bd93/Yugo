#include "pch.h"
#include "Script.h"


std::vector<Script*>& Script::GetClientScripts() 
{ 
	static std::vector<Script*> scripts;
	return scripts;
}


extern "C"
{
	SCRIPT_API std::vector<Script*> CreateScripts()
	{
		return Script::GetClientScripts();
	}

	SCRIPT_API void DeleteScripts()
	{
		auto& scripts = Script::GetClientScripts();
		for (auto script : scripts)
			delete script;
	}

}