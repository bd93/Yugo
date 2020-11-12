#include "pch.h"
#include "PlaneInterfaceImpl.h"
#include "Scene/MouseRay.h"

namespace Yugo
{

	bool PlaneInterfaceImpl::IsIntersectedWithRay(const glm::vec3& planeNormal, float distanceToCoordOrigin)
	{
		return MouseRay::CheckCollisionWithPlane(planeNormal, distanceToCoordOrigin);
	}

	glm::vec3 PlaneInterfaceImpl::GetIntersectionPoint()
	{
		return MouseRay::GetIntersectionPoint();
	}

	float PlaneInterfaceImpl::GetCollisionDistance()
	{
		return MouseRay::GetCollisionDistance();
	}

}