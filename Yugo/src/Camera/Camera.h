#pragma once
#include "Input/UserInput.h"
#include "Events/KeyboardKeyEvent.h"
#include "Events/MouseButtonEvent.h"
#include "Core/Dispatcher.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Yugo
{

	class Camera
	{
	private:
		struct MouseOffset
		{
			// Mouse position at the last frame 
			float LastX = 0;
			float LastY = 0;
			// Offset between two frames
			float OffsetX;
			float OffsetY;
		};
		MouseOffset m_MouseOffset;

		// Camera vectors
		glm::vec3 m_Position;
		glm::vec3 m_WorldUp;
		glm::vec3 m_Right;
		glm::vec3 m_Direction;
	
		// Transformation matrix for transforming camera local space to world space
		glm::mat4 m_View;
		// Projection matrix
		glm::mat4 m_Projection;
		float m_FrustrumWidth;
		float m_FrustrumHeight;
		float m_Ratio;

		// Camera angles
		float m_Yaw;
		float m_Pitch;

		// Pivot
		glm::vec3 m_Pivot;
		// Pivot distance from camera
		float m_PivotDistance;

		// Mouse movement sensitivity
		float m_Sensitivity = 0.1f;

		void RotateAroundPivot();
		void ResetMousePositionOffset();
		void RecalculateDirection();

	public:
		Camera(glm::vec3 position);
		Camera() = default;
		~Camera();

		void OnStart();
		void OnShutdown();
		void OnEvent(const Event& event);
		void OnUpdate(float timeStep);

		const glm::mat4& GetViewMatrix() const ;
		const glm::mat4& GetProjectionMatrix() const;
		const glm::vec3& GetDirection() const;
		const glm::vec3& GetPosition() const;
	};

}