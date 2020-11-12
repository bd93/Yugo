#include "pch.h"
#include "Plane.h"
#include "Scripting/PlaneInterfaceImpl.h"

#define SCRIPT_API __declspec(dllexport)

namespace GameLogic
{

	PlaneInterface* Plane::s_PlaneInterface = nullptr;

	Plane::Plane()
		: m_PlaneNormal(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_DistanceToCoordOrigin(0.0f)
	{
		
	}

	Plane::Plane(const glm::vec3& planeNormal, float distanceToCoordOrigin)
		: m_PlaneNormal(planeNormal),
		m_DistanceToCoordOrigin(distanceToCoordOrigin)
	{
	}

	bool Plane::IsIntersectedWithRay()
	{
		return s_PlaneInterface->IsIntersectedWithRay(m_PlaneNormal, m_DistanceToCoordOrigin);
	}

	glm::vec3 Plane::GetIntersectionPoint()
	{
		return s_PlaneInterface->GetIntersectionPoint();
	}

	float Plane::GetCollisionDistance()
	{
		return s_PlaneInterface->GetCollisionDistance();
	}

	void Plane::SetInterface(PlaneInterface* planeInterface)
	{
		s_PlaneInterface = planeInterface;
	}

	extern "C"
	{
		SCRIPT_API void SetPlaneInterface(PlaneInterface* planeInterface)
		{
			Plane::SetInterface(planeInterface);
		}
	}

}