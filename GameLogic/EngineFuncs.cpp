#include "pch.h"
#include "EngineFuncs.h"
#include "Common.h"


namespace GameLogic
{

	GameEngineFuncs EngineFuncs::s_GameEngineFuncs;
	UiEngineFuncs EngineFuncs::s_UiEngineFuncs;

	extern "C"
	{
		SCRIPT_API void ImportGameEngineFuncs(GameEngineFuncs importedGameEngineFuncs)
		{
			//auto& gameEngineFuncs = EngineFuncs::GetGameEngineFuncs();
			//gameEngineFuncs = importedGameEngineFuncs;
			EngineFuncs::SetGameEngineFuncs(importedGameEngineFuncs);
		}
		SCRIPT_API void ImportUiEngineFuncs(UiEngineFuncs importedUiEngineFuncs)
		{
			EngineFuncs::SetUiEngineFuncs(importedUiEngineFuncs);
		}
	}

}