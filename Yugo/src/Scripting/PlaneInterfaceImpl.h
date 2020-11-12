#pragma once
#include "Interfaces/PlaneInterface.h"


namespace Yugo
{

	class PlaneInterfaceImpl : public GameLogic::PlaneInterface
	{
	public:
		virtual bool IsIntersectedWithRay(const glm::vec3& planeNormal, float distanceToCoordOrigin) override;
		virtual glm::vec3 GetIntersectionPoint() override;
		virtual float GetCollisionDistance() override;
	};

}