#include "pch.h"
#include "Grid.h"
#include "Renderer/SpriteRenderer.h"
#include "Core/FileSystem.h"


namespace Yugo
{

	Grid::Grid(size_t numOfQuads, float quadSize, float lineThickness)
		: m_NumOfQuads(numOfQuads), m_QuadSize(quadSize), m_LineThickness(lineThickness)
	{
		SpriteRenderer::Submit(m_Quad);
		
		Texture quad{ FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Textures\\quad.png" };
		m_Quad.Texture = quad;

		m_ModelMatricesBuffer = sPtrCreate<Buffer<glm::mat4>>();
		m_ModelMatricesBuffer->Bind();

		// Create grid model matrices
		size_t rows = std::sqrt(m_NumOfQuads); // number of rows = number of columns
		size_t columns = rows;
		float firstQuadPosX = -(float)(rows / 2) * m_QuadSize;
		float firstQuadPosZ = firstQuadPosX;
		glm::vec3 firstQuadPos{ firstQuadPosX, 0.0f, firstQuadPosZ };
		for (size_t i = 0; i < rows; ++i)
		{
			float posZ = firstQuadPos.z + (i * m_QuadSize);
			for (size_t j = 0; j < columns; ++j)
			{
				glm::mat4 model{ 1.0f };
				float posX = firstQuadPos.x + (j * m_QuadSize);
				model = glm::translate(model, glm::vec3(posX, 0.0f, posZ)); // Grid is laying on "xz" plane ground
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(m_QuadSize, m_QuadSize, m_QuadSize));
				m_ModelMatrices.push_back(model);
			}
		}
		m_ModelMatricesBuffer->SetData(m_ModelMatrices);

		// Set VAO for instanced rendering
		m_Quad.ArrayOfVertices->Bind();
		m_Quad.ArrayOfVertices->AddLayout(2, 4, sizeof(glm::mat4), 0);
		m_Quad.ArrayOfVertices->AddLayout(3, 4, sizeof(glm::mat4), 1 * sizeof(glm::vec4));
		m_Quad.ArrayOfVertices->AddLayout(4, 4, sizeof(glm::mat4), 2 * sizeof(glm::vec4));
		m_Quad.ArrayOfVertices->AddLayout(5, 4, sizeof(glm::mat4), 3 * sizeof(glm::vec4));
		m_Quad.ArrayOfVertices->SetDivisor(2, 1);
		m_Quad.ArrayOfVertices->SetDivisor(3, 1);
		m_Quad.ArrayOfVertices->SetDivisor(4, 1);
		m_Quad.ArrayOfVertices->SetDivisor(5, 1);
		m_Quad.ArrayOfVertices->Unbind();
	}

	void Grid::Draw(const CameraComponent& camera, const Shader& shader)
	{
		SpriteRenderer::RenderInstanced(m_Quad, m_NumOfQuads, camera, shader);
	}

}