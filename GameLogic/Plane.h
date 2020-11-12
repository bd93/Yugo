#pragma once
#include "Interfaces/PlaneInterface.h"

#include <glm/glm.hpp>


namespace GameLogic
{

	class Plane
	{
	public:
		Plane();
		Plane(const glm::vec3& planeNormal, float distanceToCoordOrigin);

		bool IsIntersectedWithRay();
		glm::vec3 GetIntersectionPoint();
		float GetCollisionDistance();

		static void SetInterface(PlaneInterface* planeInterface);
	
	private:
		glm::vec3 m_PlaneNormal;
		float m_DistanceToCoordOrigin;
		static PlaneInterface* s_PlaneInterface;
	};

}