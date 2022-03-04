#pragma once
#include "Script.h"
#include "Ray.h"


namespace GameLogic
{

	class PlayerScript : public Script
	{
	public:
		PlayerScript();

		void OnStart() override;
		void OnUpdate(TimeStep ts) override;

		void OnMouseLeftClick() override;

		void Move();

	private:
		GameObject m_Player;
		GameObject m_Camera;
		Ray m_Ray;
	};

}
