#pragma once
#include "Scene/Components.h"
#include "Camera/Camera.h"
#include "Renderer/Shader.h"

namespace Yugo
{

	class SpriteRenderer
	{
	public:
		static void Submit(SpriteComponent& sprite);
		static void Render(const SpriteComponent& sprite, const TransformComponent& transform, const Shader& shader);
		//static void SetProjection(float left, float right, float top, float bottom, float front, float back);
		static void SetCamera(const sPtr<Camera>& camera);
		//static glm::mat4& GetProjection();
		static void EnableBlend();
		static void DisableBlend();
		static void EnableDepthTest();
		static void DisableDepthTest();
		static void ClearColorBuffer(float r, float g, float b); // r = red, g = green, b = blue
		static void ClearDepthBuffer();
	
	private:
		//static glm::mat4 s_ProjectionMat;
	};

}