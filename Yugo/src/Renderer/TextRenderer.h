#pragma once
#include "Renderer/Shader.h"
#include "Camera/Camera.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Yugo
{

	// This struct holds all state information relevant to a character which is loaded via FreeType
	struct Character
	{
		uint32_t TextureID; // ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		uint32_t Advance;   // Horizontal offset to advance to next glyph
	};

	class TextRenderer
	{
	public:
		static void Submit();
		//static void Render(TextWidgetComponent& text, TransformComponent& transform, const CameraComponent& camera, const Shader& shader);
		//static void SetCamera(const sPtr<Camera>& camera);
		static void EnableBlend();
		static void DisableBlend();
		static void EnableDepthTest();
		static void DisableDepthTest();
		static void EnableCullFace();
		static void DisableCullFace();
		static void ClearColorBuffer(float r, float g, float b); // r = red, g = green, b = blue
		static void ClearDepthBuffer();
	};

}