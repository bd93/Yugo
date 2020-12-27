#include "pch.h"
#include "EngineFuncs.h"
#include "Common.h"


namespace GameLogic
{

	extern "C"
	{
		SCRIPT_API void ImportGameEngineFuncs(GameEngineFuncs importedGameEngineFuncs)
		{
			auto& gameEngineFuncs = EngineFuncs::GetGameEngineFuncs();
			gameEngineFuncs.GetTransformComponent = importedGameEngineFuncs.GetTransformComponent;
		}
	}

}