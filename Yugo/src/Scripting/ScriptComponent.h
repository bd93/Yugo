#pragma once
#include "Scene/Components.h"


#include <entt/entt.hpp>


namespace Yugo
{

	struct ScriptComponent
	{
		std::string ScriptFilePath = "None";
		entt::entity Entity = entt::null; // Entity which this script is attached to
	};

}