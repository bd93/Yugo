#pragma once
#include "Camera/Camera.h"
#include "Components.h"

#include <glm/glm.hpp>


namespace Yugo
{

	class MouseRay
	{
	public:
		//void CalculateRayOrigin(const sPtr<Camera>& camera, float mousePosX, float mousePosY, int sceneWidth, int sceneHeight);
		//bool CheckCollisionWithPlane();
		//bool CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform);
		//bool CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform); // Check if ray intersects any triangle in mesh
		//bool CheckCollisionWithSprite(const SpriteComponent& sprite, const TransformComponent& transform);
		//glm::vec3 GetIntersectionPoint();
		//float GetCollisionDistance();

		static void CalculateRayOrigin(const sPtr<Camera>& camera, float mousePosX, float mousePosY, int sceneWidth, int sceneHeight);
		static bool CheckCollisionWithPlane();
		static bool CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform);
		static bool CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform); // Check if ray intersects any triangle in mesh
		static bool CheckCollisionWithSprite(const SpriteComponent& sprite, const TransformComponent& transform);
		static glm::vec3 GetIntersectionPoint();
		static float GetCollisionDistance();

	private:
		//glm::vec3 m_MouseRayOrigin;
		//glm::vec3 m_MouseRayDirection;
		//glm::vec3 m_MouseEyeSpaceCoords;
		//float m_MouseRayCollisionDistance;

		static glm::vec3 s_MouseRayOrigin;
		static glm::vec3 s_MouseRayDirection;
		static glm::vec3 s_MouseEyeSpaceCoords;
		static float s_MouseRayCollisionDistance;
	};

}