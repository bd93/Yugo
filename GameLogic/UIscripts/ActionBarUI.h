#pragma once
#include "Yugo.h"
#include "Input.h"


namespace GameLogic
{

	class ActionBarUI : public Script
	{
	private:
		std::vector<GameObject> Buttons;

	public:
		ActionBarUI();

		void OnStart() override;

		void OnUpdate(TimeStep ts) override;

		void OnEvent(const Event& event) override;

		void OnShutdown() override;
	};

}