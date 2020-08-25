#pragma once
#include "Camera/Camera.h"
#include "Components.h"

#include <glm/glm.hpp>


namespace Yugo
{

	class MouseRay
	{
	public:
		void CalculateRayOrigin(const sPtr<Camera>& camera, float mousePosX, float mousePosY, int sceneWidth, int sceneHeight);
		bool CheckCollisionWithPlane();
		bool CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform);
		bool CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform); // Check if ray intersects any triangle in mesh
		glm::vec3 GetIntersectionPoint();
		float GetCollisionDistance();

	private:
		glm::vec3 m_MouseRayOrigin;
		glm::vec3 m_MouseRayDirection;
		float m_MouseRayCollisionDistance;

	};

}