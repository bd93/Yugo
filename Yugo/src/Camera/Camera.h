#pragma once
#include "Input/UserInput.h"
#include "Events/KeyboardKeyEvent.h"
#include "Events/MouseButtonEvent.h"
#include "Core/Dispatcher.h"
#include "Components.h"
#include "Scene/Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Yugo
{

	class Camera
	{
	public:
		static void OnStart(TransformComponent& transform, CameraComponent& camera);

		static void Move(TimeStep ts, TransformComponent& transform, CameraComponent& camera);
		static void Scroll(float offset, CameraComponent& camera);
		static void RotateAroundPivot(TransformComponent& transform, CameraComponent& camera);
		
		static void RecalculateDirection(CameraComponent& camera);
		static void ResetMousePositionOffset(CameraComponent& camera);

		static void UpdateProjectionMatrix(CameraComponent& camera, int screenWidth, int screenHeight);
	};

}