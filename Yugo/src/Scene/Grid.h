#pragma once
#include "Components.h"
#include "Camera/Components.h"
#include "Renderer/Shader.h"


namespace Yugo
{

	class Grid
	{
	public:
		Grid(size_t numOfQuads, float quadSize, float lineThickness);
		void Draw(const CameraComponent& camera, const Shader& shader);

	private:
		size_t m_NumOfQuads;
		float m_QuadSize;
		float m_LineThickness;
		std::vector<glm::mat4> m_ModelMatrices; // Transformation matrices for each instance of a quad
		sPtr<Buffer<glm::mat4>> m_ModelMatricesBuffer; // Buffer for storing model matrices data
		SpriteComponent m_Quad; // Represents one quad in a grid
	};

}