#pragma once
#include "Scene/Components.h"
#include "Renderer/Shader.h"
#include "Camera/Camera.h"

namespace Yugo
{

	class MeshRenderer
	{
	public:
		// This static method creates vertex and index buffers and vertex arrays for Mesh (one buffer/vertex array per Submesh)
		static void Submit(MeshComponent& mesh);
		//static void Render(const MeshComponent& mesh, const TransformComponent& transform, const Shader& shader);
		static void Render(const MeshComponent& mesh, const TransformComponent& transform, const CameraComponent& camera, const Shader& shader);
		//static void SetCamera(const sPtr<Camera>& camera);
		static void DrawAABB(const BoundBoxComponent& aabb, const TransformComponent& transform, const CameraComponent& camera, const Shader& shader); // For debug purpose
		//static void DrawAABB(const BoundBoxComponent& aabb, const TransformComponent& transform, const Shader& shader); // For debug purpose
		static void EnableBlend();
		static void DisableBlend();
		static void EnableDepthTest();
		static void DisableDepthTest();
		static void ClearColorBuffer(float r, float g, float b); // r = red, g = green, b = blue
		static void ClearDepthBuffer();
	};

}