#pragma once
#include "Camera/Camera.h"
#include "Components.h"

#include <glm/glm.hpp>


namespace Yugo
{

	class MouseRay
	{
	public:
		struct RayInfo
		{
			glm::vec3 MouseRayOrigin;
			glm::vec3 MouseRayDirection;
			glm::vec3 MouseEyeSpaceCoords;
			float MouseRayCollisionDistance;
		};

	public:
		static void CalculateRayOrigin(const CameraComponent& camera, float mousePosX, float mousePosY, int sceneWidth, int sceneHeight, RayInfo* info = nullptr);
		static bool CheckCollisionWithPlane(const glm::vec3& planeNormal = glm::vec3(0.0f, 1.0f, 0.0f), float distanceToCoordOrigin = 0.0f, RayInfo* info = nullptr);
		static bool CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform, RayInfo* info = nullptr);
		static bool CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform, RayInfo* info = nullptr); // Check if ray intersects any triangle in mesh
		static bool CheckCollisionWithSprite(const TransformComponent& transform, RayInfo* info = nullptr);
		static glm::vec3 GetIntersectionPoint();
		static float GetCollisionDistance();

	private:
		static glm::vec3 s_MouseRayOrigin;
		static glm::vec3 s_MouseRayDirection;
		static glm::vec3 s_MouseEyeSpaceCoords;
		static float s_MouseRayCollisionDistance;
	};

}