#pragma once
#include "YugoComponents.h"
#include "Interfaces/MouseRayInterface.h"
#include "Scene/MouseRay.h"


namespace GameLogic
{

	class MouseRay
	{
		using RayInfo = Yugo::MouseRay::RayInfo;

	public:
		MouseRay();
		~MouseRay();

		void CastRay(const CameraComponent& camera);
		bool CheckCollisionWithPlane(const glm::vec3& planeNormal, float distanceToCoordOrigin);
		bool CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform);
		bool CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform);
		bool CheckCollisionWithSprite(const TransformComponent& transform);

		glm::vec3 GetIntersectionPoint();

		static void SetInterface(MouseRayInterface* mouseRayInterface);

	private:
		RayInfo* m_RayInfo;

		static MouseRayInterface* s_MouseRayInterface;
	};

}