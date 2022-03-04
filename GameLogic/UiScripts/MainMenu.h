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

		void SayHi();

	private:
		GameObject m_Player;
		Script* m_PlayerScript;
		Ray m_Ray;

	};

}