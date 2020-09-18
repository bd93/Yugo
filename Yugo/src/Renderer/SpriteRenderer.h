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
		static void EnableBlend();
		static void DisableBlend();
		static void EnableDepthTest();
		static void DisableDepthTest();
		static void ClearColorBuffer(float r, float g, float b); // r = red, g = green, b = blue
		static void ClearDepthBuffer();
	};

}