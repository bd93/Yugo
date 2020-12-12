#pragma once
#include "Scene/Components.h"
#include "Renderer/Shader.h"
#include "Camera/Camera.h"

namespace Yugo
{

	/**
	 * @brief Animated and non-animated mesh renderer.
	 *
	 * Methods are static and they pass components data to OpenGL rendering calls.
	 */
	class MeshRenderer
	{
	public:
		static void Submit(MeshComponent& mesh);
		static void SubmitCopy(MeshComponent& mesh);
		static void Render(const MeshComponent& mesh, const TransformComponent& transform, const CameraComponent& camera, const Shader& shader);
		static void DrawAABB(const BoundBoxComponent& aabb, const TransformComponent& transform, const CameraComponent& camera, const Shader& shader);

		//static void EnableBlend();
		//static void DisableBlend();
		//static void EnableDepthTest();
		//static void DisableDepthTest();
		//static void ClearColor(float r, float g, float b); // r = red, g = green, b = blue
		//static void ClearColorBuffer();
		//static void ClearDepthBuffer();
	};

}