#pragma once
#include "Core/Core.h"
#include "AllComponents.h"
#include "Scene/Scene.h"

namespace Yugo
{

	class PhysicsEngine
	{
	public:
		void OnStart();
		void OnUpdate(TimeStep ts);
		void OnShutDown();

		void SetScene(Scene* scene);

	private:
		Scene* m_Scene;
	};

}