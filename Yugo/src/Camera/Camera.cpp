#include "pch.h"
#include "Camera.h"


namespace Yugo
{

	Camera::Camera(glm::vec3 position)
		: m_Position(position), 
		m_Direction(glm::vec3(-1.0f / 2.0f, -sqrt(2) / 2.0f, -1.0f / 2.0f)), // 45 deg. pitch and yaw
		m_WorldUp(glm::vec3(0.0, 1.0, 0.0)), 
		m_Yaw(-135.0f), // -90(by default) - 45 = -135
		m_Pitch(-45.0f),
		m_PivotDistance(20.0f),
		m_FrustrumWidth(1200.0f),
		m_FrustrumHeight(800.0f)
	{
		//m_Projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);
		//m_Projection = glm::ortho(-600.0f, 600.0f, -400.0f, 400.0f, -400.0f, 400.0f);
		m_Projection = glm::ortho(
			-m_FrustrumWidth / 2,
			m_FrustrumWidth / 2,
			-m_FrustrumHeight / 2,
			m_FrustrumHeight / 2,
			-2000.0f, // Near clipping plane
			2000.0f   // Far clipping plane
		);

		m_Ratio = m_FrustrumWidth / m_FrustrumHeight;

		//m_Front = glm::normalize(-m_Direction);
		m_Right = glm::normalize(glm::cross(m_Direction, m_WorldUp));
		//m_Up = glm::cross(m_Front, m_Right);
		m_Pivot = m_Position + m_PivotDistance * m_Direction;

		m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_WorldUp);
	}

	Camera::~Camera()
	{
	}

	void Camera::RotateAroundPivot()
	{
		auto [mousePosX, mousePosY] = UserInput::GetMousePosition();

		if (m_MouseOffset.LastX == 0)
			m_MouseOffset.LastX = mousePosX;

		if (m_MouseOffset.LastY == 0)
			m_MouseOffset.LastY = mousePosY;

		m_MouseOffset.OffsetX = mousePosX - m_MouseOffset.LastX;
		m_MouseOffset.OffsetY = -mousePosY + m_MouseOffset.LastY;

		m_MouseOffset.LastX = mousePosX;
		m_MouseOffset.LastY = mousePosY;

		RecalculateDirection();

		if (m_MouseOffset.OffsetX != 0 || m_MouseOffset.OffsetY != 0)
		{
			m_Position.x = m_Pivot.x - m_PivotDistance * m_Direction.x;
			m_Position.y = m_Pivot.y - m_PivotDistance * m_Direction.y;
			m_Position.z = m_Pivot.z - m_PivotDistance * m_Direction.z;
		}

		m_View = glm::lookAt(m_Position, m_Pivot, m_WorldUp);
	}

	void Camera::OnStart()
	{
		Dispatcher::Subscribe<KeyboardKeyPress>(this);
		Dispatcher::Subscribe<MouseButtonPress>(this);
	}

	void Camera::OnShutdown()
	{
	}

	void Camera::OnEvent(const Event& event)
	{
		if (event.GetEventType() == EventType::MouseButtonPress)
		{
			const auto& mouseButtonPress = static_cast<const MouseButtonPress&>(event);
			if (mouseButtonPress.GetButtonCode() == MOUSE_BUTTON_LEFT)
				ResetMousePositionOffset();
		}

		if (event.GetEventType() == EventType::MouseScroll)
		{
			const auto& mouseScroll = static_cast<const MouseScroll&>(event);

			m_FrustrumHeight += mouseScroll.GetOffsetY() * 20.0f;
			if (m_FrustrumHeight < 200.0f)
			{
				m_FrustrumHeight = 200.0f;
				return;
			}
			if (m_FrustrumHeight > 2000.0f)
			{
				m_FrustrumHeight = 2000.0f;
				return;
			}
			m_FrustrumWidth += mouseScroll.GetOffsetY() * 20.0f;

			m_Projection = glm::ortho(
				-m_FrustrumWidth / 2, 
				m_FrustrumWidth / 2, 
				-m_FrustrumHeight / 2, 
				m_FrustrumHeight / 2, 
				-2000.0f, // Near clipping plane
				2000.0f   // Far clipping plane
			);
		}
	}

	void Camera::OnUpdate(float timeStep)
	{
		if (UserInput::IsKeyboardKeyPressed(KEY_UP))
		{
			//m_Position += 2.5f * timeStep * m_Direction;
			m_Position -= 200.0f * timeStep * glm::vec3(1.0f, 0.0f, 1.0f);
			m_Pivot = m_Position + m_PivotDistance * m_Direction;
			m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_WorldUp);
		}

		if (UserInput::IsKeyboardKeyPressed(KEY_DOWN))
		{
			//m_Position -= 2.5f * timeStep * m_Direction;
			m_Position += 200.0f * timeStep * glm::vec3(1.0f, 0.0f, 1.0f);
			m_Pivot = m_Position + m_PivotDistance * m_Direction;
			m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_WorldUp);
		}

		if (UserInput::IsKeyboardKeyPressed(KEY_LEFT))
		{
			m_Position -= 200.0f * timeStep * m_Right;
			m_Pivot = m_Position + m_PivotDistance * m_Direction;
			m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_WorldUp);
		}

		if (UserInput::IsKeyboardKeyPressed(KEY_RIGHT))
		{
			m_Position += 200.0f * timeStep * m_Right;
			m_Pivot = m_Position + m_PivotDistance * m_Direction;
			m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_WorldUp);
		}

		if (UserInput::IsKeyboardKeyPressed(KEY_LEFT_CONTROL) && UserInput::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			RotateAroundPivot();
		}
	}

	void Camera::RecalculateDirection()
	{
		float yaw = m_Sensitivity * m_MouseOffset.OffsetX;
		m_Yaw += yaw;

		float pitch = m_Sensitivity * m_MouseOffset.OffsetY;
		m_Pitch += pitch;
		
		if (m_Pitch > 89.0f)
		{
			m_Pitch = 89.0f;
		}
		if (m_Pitch < -89.0f)
		{
			m_Pitch = -89.0f;
		}

		m_Direction.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
		m_Direction.y = glm::sin(glm::radians(m_Pitch));
		m_Direction.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));

		m_Right = glm::normalize(glm::cross(m_Direction, m_WorldUp));
	}

	const glm::mat4& Camera::GetViewMatrix() const
	{
		return m_View;
	}

	const glm::mat4& Camera::GetProjectionMatrix() const
	{
		return m_Projection;
	}

	const glm::vec3& Camera::GetDirection() const
	{
		return m_Direction;
	}

	const glm::vec3& Camera::GetPosition() const
	{
		return m_Position;
	}

	void Camera::ResetMousePositionOffset()
	{
		m_MouseOffset.LastX = 0.0f;
		m_MouseOffset.LastY = 0.0f;
		m_MouseOffset.OffsetX = 0.0f;
		m_MouseOffset.OffsetY = 0.0f;
	}

}