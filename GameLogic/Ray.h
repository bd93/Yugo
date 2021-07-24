#pragma once
#include "Yugo.h"


namespace GameLogic
{

	class Ray
	{
	public:
		void CastRay(CameraComponent& camera, float mousePosX, float mousePosY);
		bool CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform);
		bool CheckCollisionWithPlane(const glm::vec3& planeNormal, float distanceToCoordOrigin);
		const glm::vec3& GetIntersectionPoint();

	private:
		Yugo::MouseRay::RayInfo m_RayInfo;
		glm::vec3 m_IntersectionPoint;
		glm::vec3 m_MouseRayOrigin;
		glm::vec3 m_MouseRayDirection;
	};

}