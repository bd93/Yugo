#pragma once




namespace Yugo
{

	struct CameraComponent
	{
		struct MouseOffset
		{
			// Mouse position at the last frame 
			float LastX = 0;
			float LastY = 0;
			// Offset between two frames
			float OffsetX;
			float OffsetY;
		};
		MouseOffset MouseOffset;

		// Camera vectors
		//glm::vec3 Position;
		glm::vec3 WorldUp;
		glm::vec3 Right;
		glm::vec3 Direction;

		// Transformation matrix for transforming camera local space to world space
		glm::mat4 View;
		// Projection matrix
		glm::mat4 Projection;
		float FrustrumWidth;
		float FrustrumHeight;
		float Ratio;

		// Camera angles
		float Yaw;
		float Pitch;

		// Pivot
		glm::vec3 Pivot;
		// Pivot distance from camera
		float PivotDistance;

		// Mouse movement sensitivity
		float Sensitivity = 0.1f;
	};

}