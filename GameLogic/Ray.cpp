#include "pch.h"
#include "Ray.h"
#include "EngineFuncs.h"


namespace GameLogic
{

	void Ray::CastRay(CameraComponent& camera, float mousePosX, float mousePosY)
	{
		auto [windowWidth, windowHeight] = EngineFuncs::GetGameEngineFuncs().GetWindowSize();
		EngineFuncs::GetGameEngineFuncs().CalculateRayOrigin(camera, mousePosX, mousePosY, windowWidth, windowHeight, &m_RayInfo);
	}

	bool Ray::CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform)
	{
		return EngineFuncs::GetGameEngineFuncs().CheckCollisionWithMesh(mesh, transform, &m_RayInfo);
	}

	bool Ray::CheckCollisionWithPlane(const glm::vec3& planeNormal, float distanceToCoordOrigin)
	{
		return EngineFuncs::GetGameEngineFuncs().CheckCollisionWithPlane(planeNormal, distanceToCoordOrigin, &m_RayInfo);
	}

	const glm::vec3& Ray::GetIntersectionPoint()
	{
		return m_RayInfo.MouseRayOrigin + m_RayInfo.MouseRayCollisionDistance * m_RayInfo.MouseRayDirection;
	}

}