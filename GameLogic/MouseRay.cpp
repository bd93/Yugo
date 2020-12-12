#include "pch.h"
#include "MouseRay.h"
#include "Input.h"

#define SCRIPT_API __declspec(dllexport)


namespace GameLogic
{

	MouseRayInterface* MouseRay::s_MouseRayInterface = nullptr;

	MouseRay::MouseRay()
		: m_RayInfo(new RayInfo())
	{
	}

	MouseRay::~MouseRay()
	{
		delete m_RayInfo;
	}

	void MouseRay::CastRay(const CameraComponent& camera)
	{
		auto [mousePosX, mousePosY] = Input::GetMousePosition();
		auto [screenWidth, screenHeight] = Input::GetScreenDimension();
		s_MouseRayInterface->CastRay(camera, mousePosX, mousePosY, screenWidth, screenHeight, m_RayInfo);
	}

	bool MouseRay::CheckCollisionWithPlane(const glm::vec3& planeNormal, float distanceToCoordOrigin)
	{
		return s_MouseRayInterface->CheckCollisionWithPlane(planeNormal, distanceToCoordOrigin, m_RayInfo);
	}

	bool MouseRay::CheckCollisionWithBox(const MeshComponent& mesh, const TransformComponent& transform)
	{
		return s_MouseRayInterface->CheckCollisionWithBox(mesh, transform, m_RayInfo);
	}

	bool MouseRay::CheckCollisionWithMesh(const MeshComponent& mesh, const TransformComponent& transform)
	{
		return s_MouseRayInterface->CheckCollisionWithMesh(mesh, transform, m_RayInfo);
	}

	bool MouseRay::CheckCollisionWithSprite(const TransformComponent& transform)
	{
		return s_MouseRayInterface->CheckCollisionWithSprite(transform, m_RayInfo);
	}

	glm::vec3 MouseRay::GetIntersectionPoint()
	{
		return m_RayInfo->MouseRayOrigin + m_RayInfo->MouseRayCollisionDistance * m_RayInfo->MouseRayDirection;
	}

	void MouseRay::SetInterface(MouseRayInterface* mouseRayInterface)
	{
		s_MouseRayInterface = mouseRayInterface;
	}

	extern "C"
	{
		SCRIPT_API void SetMouseRayInterface(MouseRayInterface* mouseRayInterface)
		{
			MouseRay::SetInterface(mouseRayInterface);
		}
	}

}