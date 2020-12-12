#pragma once
#include "Interfaces/MouseRayInterface.h"


namespace Yugo
{

	class MouseRayInterfaceImpl : public GameLogic::MouseRayInterface
	{
	public:
		virtual void CastRay(const CameraComponent& camera, float mousePosX, float mousePosY, int screenWidth, int screenHeight, RayInfo* info = nullptr) override;
		virtual bool CheckCollisionWithPlane(const glm::vec3& planeNormal, float distanceToCoordOrigin, RayInfo* info = nullptr) override;
		virtual bool CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform, RayInfo* info = nullptr) override;
		virtual bool CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform, RayInfo* info = nullptr) override;
		virtual bool CheckCollisionWithSprite(const TransformComponent& transform, RayInfo* info = nullptr) override;
		virtual glm::vec3 GetIntersectionPoint() override;

	};

}