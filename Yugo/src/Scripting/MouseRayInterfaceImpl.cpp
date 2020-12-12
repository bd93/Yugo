#include "pch.h"
#include "MouseRayInterfaceImpl.h"
#include "Scene/MouseRay.h"


namespace Yugo
{

	void MouseRayInterfaceImpl::CastRay(const CameraComponent& camera, float mousePosX, float mousePosY, int screenWidth, int screenHeight, RayInfo* info)
	{
		MouseRay::CalculateRayOrigin(camera, mousePosX, mousePosY, screenWidth, screenHeight, info);
	}

	bool MouseRayInterfaceImpl::CheckCollisionWithPlane(const glm::vec3& planeNormal, float distanceToCoordOrigin, RayInfo* info)
	{
		return MouseRay::CheckCollisionWithPlane(planeNormal, distanceToCoordOrigin, info);
	}

	bool MouseRayInterfaceImpl::CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform, RayInfo* info)
	{
		return MouseRay::CheckCollisionWithBox(mesh, transform, info);
	}

	bool MouseRayInterfaceImpl::CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform, RayInfo* info)
	{
		return MouseRay::CheckCollisionWithMesh(mesh, transform, info);
	}

	bool MouseRayInterfaceImpl::CheckCollisionWithSprite(const TransformComponent& transform, RayInfo* info)
	{
		return MouseRay::CheckCollisionWithSprite(transform, info);
	}

	glm::vec3 MouseRayInterfaceImpl::GetIntersectionPoint()
	{
		return MouseRay::GetIntersectionPoint();
	}

}