#pragma once
#include "YugoEvents.h"
#include "YugoComponents.h"
#include "Scene/MouseRay.h"


namespace GameLogic
{

	class MouseRayInterface
	{
	protected:
		using RayInfo = Yugo::MouseRay::RayInfo;

	public:
		virtual void CastRay(const CameraComponent& camera, float mousePosX, float mousePosY, int screenWidth, int screenHeight, RayInfo* info = nullptr) = 0;
		virtual bool CheckCollisionWithPlane(const glm::vec3& planeNormal, float distanceToCoordOrigin, RayInfo* info = nullptr) = 0;
		virtual bool CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform, RayInfo* info = nullptr) = 0;
		virtual bool CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform, RayInfo* info = nullptr) = 0;
		virtual bool CheckCollisionWithSprite(const TransformComponent& transform, RayInfo* info = nullptr) = 0;
		virtual glm::vec3 GetIntersectionPoint() = 0;
	};

}