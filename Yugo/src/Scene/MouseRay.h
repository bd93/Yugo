#pragma once
#include "Camera/Camera.h"
#include "Components.h"

#include <glm/glm.hpp>


namespace Yugo
{

	class MouseRay
	{
	public:
		static void CalculateRayOrigin(const sPtr<Camera>& camera, float mousePosX, float mousePosY, int sceneWidth, int sceneHeight);
		static bool CheckCollisionWithPlane();
		static bool CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform);
		static bool CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform); // Check if ray intersects any triangle in mesh
		static bool CheckCollisionWithSprite(const TransformComponent& transform);
		static glm::vec3 GetIntersectionPoint();
		static float GetCollisionDistance();

	private:
		static glm::vec3 s_MouseRayOrigin;
		static glm::vec3 s_MouseRayDirection;
		static glm::vec3 s_MouseEyeSpaceCoords;
		static float s_MouseRayCollisionDistance;
	};

}