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
		static void Render(const SpriteComponent& sprite, const TransformComponent& transform, const CameraComponent& camera, const Shader& shader);
		static void RenderInstanced(const SpriteComponent& sprite, size_t numOfInstances, const CameraComponent& camera, const Shader& shader);
	};

}