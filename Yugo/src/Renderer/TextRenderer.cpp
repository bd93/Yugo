#include "pch.h"
#include "TextRenderer.h"
#include "Core/ResourceManager.h"
#include "Core/FileSystem.h"
#include "Core/Core.h"


namespace Yugo
{

    static std::map<char, Character> s_Characters;
    static uint32_t s_VAO, s_VBO;
    //static sPtr<Camera> s_Camera;

	void TextRenderer::Submit()
	{
		Shader textShader(
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\text_vertex.shader",
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\text_fragment.shader"
		);
		ResourceManager::AddShader("textShader", textShader);

		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			assert(0);
		}

		std::string font_name = FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Fonts\\arial.ttf";
		if (font_name.empty())
		{
			std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
			assert(0);
		}

        FT_Face face;
        if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            assert(0);
        }
        else {
            // Set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // Disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // Load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // Generate texture
                uint32_t texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // Set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // Store character for later use
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                s_Characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // Destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        glGenVertexArrays(1, &s_VAO);
        glGenBuffers(1, &s_VBO);
        glBindVertexArray(s_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}

	void TextRenderer::Render(TextWidgetComponent& text, TransformComponent& transform, const CameraComponent& camera, const Shader& shader)
	{
        EnableCullFace();
        EnableBlend();

        // Activate corresponding render state	
        shader.Use();
        shader.SetVec3("textColor", text.Color);
        shader.SetMat4("projection", camera.Projection);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(s_VAO);

        float x = transform.Position.x;
        float y = transform.Position.y;
        float scale = transform.Scale.y;
        float textWidth = 0.0f;

        // Calculate text width (for text center position inside parent widget)
        float xPosFirstCharBegin = text.Text.size() == 0 ? 0.0f : x;
        float xPosLastCharEnd = xPosFirstCharBegin;
        for (auto c : text.Text)
        {
            Character& ch = s_Characters[c];
            xPosLastCharEnd += (ch.Advance >> 6) * scale;
        }
        textWidth = xPosLastCharEnd - xPosFirstCharBegin;
        x -= textWidth / 2.0f; // Adjusted position for centered text

        // Iterate through all characters
        for (auto c : text.Text)
        {
            Character& ch = s_Characters[c];

            float xPos = x + ch.Bearing.x * scale;
            float yPos = y - ch.Size.y * scale - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            // Update s_VBO for each character
            float vertices[6][4] = {
                { xPos,     yPos + h,   0.0f, 0.0f },
                { xPos,     yPos,       0.0f, 1.0f },
                { xPos + w, yPos,       1.0f, 1.0f },

                { xPos,     yPos + h,   0.0f, 0.0f },
                { xPos + w, yPos,       1.0f, 1.0f },
                { xPos + w, yPos + h,   1.0f, 0.0f }
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        DisableCullFace();
        DisableBlend();
	}

    //void TextRenderer::SetCamera(const sPtr<Camera>& camera)
    //{
    //    s_Camera = camera;
    //}

	void TextRenderer::EnableBlend()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void TextRenderer::DisableBlend()
	{
		glDisable(GL_BLEND);
	}

	void TextRenderer::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void TextRenderer::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

    void TextRenderer::EnableCullFace()
    {
        glEnable(GL_CULL_FACE);
    }

    void TextRenderer::DisableCullFace()
    {
        glDisable(GL_CULL_FACE);
    }

	void TextRenderer::ClearColorBuffer(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void TextRenderer::ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}