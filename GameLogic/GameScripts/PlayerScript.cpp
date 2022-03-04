#include "pch.h"
#include "Yugo.h"
#include "PlayerScript.h"


namespace GameLogic
{
	
	void PlayerScript::OnStart()
	{
		m_Camera = GameObject::FindGameObjectWithTagName("Main Camera");
		m_Player = GameObject::FindGameObjectWithTagName("Main Player");
	}

	void PlayerScript::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyboardKeyPressed(KEY_W))
		{
			auto& transform = m_GameObject.GetComponent<TransformComponent>();
			transform.Position += glm::vec3(5.5f, 0.0f, 0.0f);
		}

	}

	void PlayerScript::OnMouseLeftClick()
	{
		m_Ray.CastRay(m_Camera.GetComponent<CameraComponent>(), Input::GetMousePosX(), Input::GetMousePosY());
		if (m_Ray.CheckCollisionWithMesh(m_Player.GetComponent<MeshComponent>(), m_Player.GetComponent<TransformComponent>()))
		{
			std::cout << "mesh selected!\n";
		}

		//auto [posX, posY] = Input::GetMousePosition();
	}

	void PlayerScript::Move()
	{
		auto& transform = m_GameObject.GetComponent<TransformComponent>();
		transform.Position += glm::vec3(5.5f, 0.0f, 0.0f);
	}



	REGISTER_CLASS(PlayerScript, __FILE__)

}