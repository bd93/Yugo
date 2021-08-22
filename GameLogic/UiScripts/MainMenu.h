#pragma once
#include "UiScript.h"
#include "Ray.h"


namespace GameLogic
{

	class MainMenu : public UiScript
	{
	public:
		MainMenu();

		void OnStart() override;
		void OnUpdate(TimeStep ts) override;

		void OnMouseLeftClick() override;

	private:
		GameObject m_Player;
		Ray m_Ray;
	};

}