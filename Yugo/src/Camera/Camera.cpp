#include "pch.h"
#include "Camera.h"


namespace Yugo
{

	void Camera::OnStart(TransformComponent& transform, CameraComponent& camera)
	{
		transform.Position = glm::vec3(100.0f, 100.0f, 100.0f);

		camera.Direction = glm::vec3(-1.0f / 2.0f, -sqrt(2) / 2.0f, -1.0f / 2.0f); // 45 deg. pitch and yaw
		camera.WorldUp = glm::vec3(0.0, 1.0, 0.0);
		camera.Yaw = -135.0f; // -90(by default) - 45 = -135
		camera.Pitch = -45.0f;
		camera.PivotDistance = 20.0f;
		camera.FrustrumWidth = 1200.0f;
		camera.FrustrumHeight = 800.0f;
		
		//camera.Projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);
		//camera.Projection = glm::ortho(-600.0f, 600.0f, -400.0f, 400.0f, -400.0f, 400.0f);
		camera.Projection = glm::ortho(
			-camera.FrustrumWidth / 2,
			camera.FrustrumWidth / 2,
			-camera.FrustrumHeight / 2,
			camera.FrustrumHeight / 2,
			-2000.0f, // Near clipping plane
			2000.0f   // Far clipping plane
		);

		camera.Ratio = camera.FrustrumWidth / camera.FrustrumHeight;

		//camera.Front = glm::normalize(-m_Direction);
		camera.Right = glm::normalize(glm::cross(camera.Direction, camera.WorldUp));
		//camera.Up = glm::cross(m_Front, m_Right);
		camera.Pivot = transform.Position + camera.PivotDistance * camera.Direction;
		camera.View = glm::lookAt(transform.Position, transform.Position + camera.Direction, camera.WorldUp);
	}

	void Camera::Move(TimeStep ts, TransformComponent& transform, CameraComponent& camera)
	{
		if (UserInput::IsKeyboardKeyPressed(KEY_UP))
		{
			//m_Position += 2.5f * timeStep * m_Direction;
			transform.Position -= 200.0f * ts * glm::vec3(1.0f, 0.0f, 1.0f);
			camera.Pivot = transform.Position + camera.PivotDistance * camera.Direction;
			camera.View = glm::lookAt(transform.Position, transform.Position + camera.Direction, camera.WorldUp);
		}

		if (UserInput::IsKeyboardKeyPressed(KEY_DOWN))
		{
			//m_Position -= 2.5f * timeStep * m_Direction;
			transform.Position += 200.0f * ts * glm::vec3(1.0f, 0.0f, 1.0f);
			camera.Pivot = transform.Position + camera.PivotDistance * camera.Direction;
			camera.View = glm::lookAt(transform.Position, transform.Position + camera.Direction, camera.WorldUp);
		}

		if (UserInput::IsKeyboardKeyPressed(KEY_LEFT))
		{
			transform.Position -= 200.0f * ts * camera.Right;
			camera.Pivot = transform.Position + camera.PivotDistance * camera.Direction;
			camera.View = glm::lookAt(transform.Position, transform.Position + camera.Direction, camera.WorldUp);
		}

		if (UserInput::IsKeyboardKeyPressed(KEY_RIGHT))
		{
			transform.Position += 200.0f * ts * camera.Right;
			camera.Pivot = transform.Position + camera.PivotDistance * camera.Direction;
			camera.View = glm::lookAt(transform.Position, transform.Position + camera.Direction, camera.WorldUp);
		}
	}

	void Camera::Scroll(float offset, CameraComponent& camera)
	{
		camera.FrustrumHeight += offset * 20.0f;
		if (camera.FrustrumHeight < 200.0f)
		{
			camera.FrustrumHeight = 200.0f;
			return;
		}
		if (camera.FrustrumHeight > 2000.0f)
		{
			camera.FrustrumHeight = 2000.0f;
			return;
		}
		camera.FrustrumWidth += offset * 20.0f;

		camera.Projection = glm::ortho(
			-camera.FrustrumWidth / 2, 
			camera.FrustrumWidth / 2, 
			-camera.FrustrumHeight / 2, 
			camera.FrustrumHeight / 2, 
			-2000.0f, // Near clipping plane
			2000.0f   // Far clipping plane
		);
	}

	void Camera::RotateAroundPivot(TransformComponent& transform, CameraComponent& camera)
	{
		auto [mousePosX, mousePosY] = UserInput::GetMousePosition();

		if (camera.MouseOffset.LastX == 0)
			camera.MouseOffset.LastX = mousePosX;

		if (camera.MouseOffset.LastY == 0)
			camera.MouseOffset.LastY = mousePosY;

		camera.MouseOffset.OffsetX = mousePosX - camera.MouseOffset.LastX;
		camera.MouseOffset.OffsetY = -mousePosY + camera.MouseOffset.LastY;

		camera.MouseOffset.LastX = mousePosX;
		camera.MouseOffset.LastY = mousePosY;

		RecalculateDirection(camera);

		if (camera.MouseOffset.OffsetX != 0 || camera.MouseOffset.OffsetY != 0)
		{
			transform.Position.x = camera.Pivot.x - camera.PivotDistance * camera.Direction.x;
			transform.Position.y = camera.Pivot.y - camera.PivotDistance * camera.Direction.y;
			transform.Position.z = camera.Pivot.z - camera.PivotDistance * camera.Direction.z;
		}

		camera.View = glm::lookAt(transform.Position, camera.Pivot, camera.WorldUp);
	}

	void Camera::RecalculateDirection(CameraComponent& camera)
	{
		float yaw = camera.Sensitivity * camera.MouseOffset.OffsetX;
		camera.Yaw += yaw;

		float pitch = camera.Sensitivity * camera.MouseOffset.OffsetY;
		camera.Pitch += pitch;
			
		if (camera.Pitch > 89.0f)
		{
			camera.Pitch = 89.0f;
		}
		if (camera.Pitch < -89.0f)
		{
			camera.Pitch = -89.0f;
		}

		camera.Direction.x = glm::cos(glm::radians(camera.Yaw)) * glm::cos(glm::radians(camera.Pitch));
		camera.Direction.y = glm::sin(glm::radians(camera.Pitch));
		camera.Direction.z = glm::sin(glm::radians(camera.Yaw)) * glm::cos(glm::radians(camera.Pitch));

		camera.Right = glm::normalize(glm::cross(camera.Direction, camera.WorldUp));
	}

	void Camera::ResetMousePositionOffset(CameraComponent& camera)
	{
		camera.MouseOffset.LastX = 0.0f;
		camera.MouseOffset.LastY = 0.0f;
		camera.MouseOffset.OffsetX = 0.0f;
		camera.MouseOffset.OffsetY = 0.0f;
	}

}