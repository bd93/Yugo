//#include "pch.h"
//#include "Yugo.h"
//#include "PlayerScript.h"
//#include "Input.h"
//
//namespace GameLogic
//{
//
//	void PlayerScript::OnStart()
//	{
//		m_Enemies = GameObject::FindGameObjectsWithTag("sphere");
//		m_Camera = GameObject::FindGameObjectWithTag("Main Camera");
//
//		auto& playerTransform = m_GameObject->GetComponent<TransformComponent>();
//		auto& cameraTransform = m_Camera.GetComponent<TransformComponent>();
//		auto& camera = m_Camera.GetComponent<CameraComponent>();
//		cameraTransform.Position.x = playerTransform.Position.x + 100.0f;
//		cameraTransform.Position.z = playerTransform.Position.z + 100.0f;
//		camera.View = glm::lookAt(cameraTransform.Position, cameraTransform.Position + camera.Direction, camera.WorldUp);
//	}
//
//	void PlayerScript::OnUpdate(TimeStep ts)
//	{
//		if (Input::IsKeyboardkeyPressed(KEY_W))
//		{
//			auto& playerTransform = m_GameObject->GetComponent<TransformComponent>();
//			playerTransform.Position.x -= 5.0f;
//
//			for (auto& enemy : m_Enemies)
//			{
//				auto& transform = enemy.GetComponent<TransformComponent>();
//				transform.Position.x += 1.0f;
//			}
//		}
//
//		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
//		{
//			auto& cameraComponent = m_Camera.GetComponent<CameraComponent>();
//			m_MouseRay.CastRay(cameraComponent);
//			m_MouseRay.CheckCollisionWithPlane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
//			const auto& intersectionPoint = m_MouseRay.GetIntersectionPoint();
//
//			m_IsMoving = true;
//			m_MoveDestination = m_Plane.GetIntersectionPoint();
//			auto& transform = m_GameObject->GetComponent<TransformComponent>();
//			m_MoveDirection = glm::normalize(m_MoveDestination - transform.Position);
//		}
//
//		if (m_IsMoving)
//			Move(ts);
//	}
//
//	void PlayerScript::OnEvent(const Event& event)
//	{
//		if (event.GetEventType() == EventType::MouseButtonPress)
//		{
//			//auto& cameraComponent = m_Camera.GetComponent<CameraComponent>();
//			//m_MouseRay.CastRay(cameraComponent);
//		}
//	}
//
//	void PlayerScript::OnShutdown()
//	{
//	}
//
//	void PlayerScript::Move(TimeStep ts)
//	{
//		auto& playerTransform = m_GameObject->GetComponent<TransformComponent>();
//		glm::vec3 distance = glm::abs(m_MoveDestination - playerTransform.Position);
//		if (distance.x < 1.0f && distance.z < 1.0f)
//		{
//			m_IsMoving = false;
//		}
//		else
//		{
//			playerTransform.Position += m_Speed * ts * m_MoveDirection;
//			auto& cameraTransform = m_Camera.GetComponent<TransformComponent>();
//			auto& camera = m_Camera.GetComponent<CameraComponent>();
//			cameraTransform.Position.x += m_Speed * ts * m_MoveDirection.x;
//			cameraTransform.Position.z += m_Speed * ts * m_MoveDirection.z;
//			camera.View = glm::lookAt(cameraTransform.Position, cameraTransform.Position + camera.Direction, camera.WorldUp);
//		}
//	}
//
//
//
//	REGISTER_CLASS(PlayerScript, __FILE__)
//
//}



#include "pch.h"
#include "Yugo.h"
#include "PlayerScript.h"

namespace GameLogic
{
	
	void PlayerScript::OnStart()
	{
		auto& transform = m_GameObject.GetComponent<TransformComponent>();
	}

	void PlayerScript::OnUpdate(TimeStep ts)
	{
	}

	void PlayerScript::OnEvent(const Event& event)
	{
		if (event.GetEventType() == EventType::MouseButtonPress)
		{
			//auto& cameraComponent = m_Camera.GetComponent<CameraComponent>();
			//m_MouseRay.CastRay(cameraComponent);
		}
	}

	void PlayerScript::OnShutdown()
	{
	}


	REGISTER_CLASS(PlayerScript, __FILE__)

}