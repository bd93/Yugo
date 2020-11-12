#pragma once

#include <entt/entt.hpp>

// Forward declaration to avoid cyclic dependency
namespace GameLogic
{
	class Script;
}


namespace Yugo
{

	struct ScriptComponent
	{
		GameLogic::Script* ClientScript = nullptr;
		std::string ScriptFilePath = "None";
		//entt::entity Entity = entt::null; // Entity which this script is attached to
	};

}