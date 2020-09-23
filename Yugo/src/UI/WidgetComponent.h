#pragma once
#include "Core/Core.h"
#include "Renderer/Vertex.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

namespace Yugo
{

	struct WidgetComponent
	{
		int data;
		WidgetComponent() = default;
		WidgetComponent(const WidgetComponent& other) = default;
		// TODO: Think about adding something that is upgrade on top of SpriteComponent, which UI uses as a widget
	};

}