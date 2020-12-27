#pragma once
#include "YugoComponents.h"

#include <functional>


namespace GameLogic
{

	struct GameEngineFuncs
	{
		std::function<TransformComponent& (entt::entity)> GetTransformComponent;
	};


	struct UiEngineFuncs
	{

	};


	class EngineFuncs
	{
	public:
		static GameEngineFuncs& GetGameEngineFuncs()
		{
			static GameEngineFuncs gameEngineFuncs;
			return gameEngineFuncs;
		}
		
		static UiEngineFuncs& GetUiEngineFuncs()
		{
			static UiEngineFuncs uiEngineFuncs;
			return uiEngineFuncs;
		}
	};

}