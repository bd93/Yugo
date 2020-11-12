#pragma once
#include <glm/glm.hpp>

namespace GameLogic
{

	class PlaneInterface
	{
	public:
		virtual bool IsIntersectedWithRay(const glm::vec3& planeNormal, float distanceToCoordOrigin) = 0;
		virtual glm::vec3 GetIntersectionPoint() = 0;
		virtual float GetCollisionDistance() = 0;
	};

}