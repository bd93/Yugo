#pragma once
#include "Script.h"


namespace GameLogic
{

	class PlayerScript : public Script
	{
	private:
		std::vector<GameObject> m_Enemies;
		GameObject m_Camera;
		Plane m_Plane;
		MouseRay m_MouseRay;
		glm::vec3 m_MoveDestination;
		glm::vec3 m_MoveDirection;
		float m_Speed = 200.0f;
		bool m_IsMoving = false;

	public:
		PlayerScript();

		void OnStart() override;
		void OnUpdate(TimeStep ts) override;
		void OnEvent(const Event& event) override;
		void OnShutdown() override;

		void Move(TimeStep ts);
	};

}
