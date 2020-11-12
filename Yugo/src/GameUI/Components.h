#pragma once
#include "Core/Core.h"
#include "Renderer/Vertex.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

namespace Yugo
{

	struct CanvasWidgetComponent
	{
		int IsActive = true;
		//entt::entity Entity;
		CanvasWidgetComponent() = default;
		CanvasWidgetComponent(const CanvasWidgetComponent& other) = default;
	};

	struct ButtonWidgetComponent
	{
		bool IsClicked = false;
		int IsActive = true;
		//entt::entity Entity;
		ButtonWidgetComponent() = default;
		ButtonWidgetComponent(const ButtonWidgetComponent& other) = default;
	};

	struct TextWidgetComponent
	{
		std::string Text = "";
		glm::vec3 Color = glm::vec3(0.8f, 0.8f, 0.8f);
		int IsActive = true;
		//entt::entity Entity;
		TextWidgetComponent() = default;
		TextWidgetComponent(const TextWidgetComponent& other) = default;
	};

}