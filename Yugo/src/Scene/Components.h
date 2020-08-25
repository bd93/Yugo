#pragma once
#include "Core/Core.h"
#include "Renderer/Vertex.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

#include <glm/glm.hpp>


namespace Yugo
{

	struct Material
	{
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		glm::vec3 Ambient;
		float Shininess;
	};

	struct MeshComponent
	{
		std::vector<Vertex1P1N1T4J> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<Texture> Textures;
		Material Material;
		glm::vec3 MinAABB; // Minimum coordinates of a box vertex point (lower/left/back) 
		glm::vec3 MaxAABB; // Maximum coordinates of a box vertex point (upper/right/front)
		std::vector<MeshComponent> Submeshes;
		
		bool HasAnimation = false;
		std::vector<glm::mat4> FinalTransforms;

		sPtr<Buffer<Vertex1P1N1T4J>> VertexBuffer;
		sPtr<Buffer<unsigned int>> IndexBuffer;
		sPtr<VertexArray<Vertex1P1N1T4J>> ArrayOfVertices;
	};

	struct TransformComponent
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;
		glm::mat4 ModelMatrix;
	};

	// Axis Aligned Bounding Box
	struct BoundBoxComponent
	{
		BoundBoxComponent(const MeshComponent& mesh)
		{
			const auto& modelMinAABB = mesh.MinAABB;
			const auto& modelMaxAABB = mesh.MaxAABB;

			/*

				Box vertices:

				5 _____________6
				 /|			  /|
			   8/_|________7_/ |
				| |			|  |
				| |1________|__|2
				| /			| /
			   4|/__________|/3

			*/

			// For empty box only 5 faces are enough to draw in order to draw all edges using GL_LINE_LOOP
			Vertices = {
				// Face 1 (Vertices 1,2,3,4)
				{glm::vec3(modelMinAABB.x, modelMinAABB.y, modelMinAABB.z)},
				{glm::vec3(modelMaxAABB.x, modelMinAABB.y, modelMinAABB.z)},
				{glm::vec3(modelMaxAABB.x, modelMinAABB.y, modelMaxAABB.z)},
				{glm::vec3(modelMinAABB.x, modelMinAABB.y, modelMaxAABB.z)},

				// Face 2 (Vertices 8,5,6,7)
				{glm::vec3(modelMinAABB.x, modelMaxAABB.y, modelMaxAABB.z)},
				{glm::vec3(modelMinAABB.x, modelMaxAABB.y, modelMinAABB.z)},
				{glm::vec3(modelMaxAABB.x, modelMaxAABB.y, modelMinAABB.z)},
				{glm::vec3(modelMaxAABB.x, modelMaxAABB.y, modelMaxAABB.z)},

				// Face 3 (Vertices 8,7,3,4)
				{glm::vec3(modelMinAABB.x, modelMaxAABB.y, modelMaxAABB.z)},
				{glm::vec3(modelMaxAABB.x, modelMaxAABB.y, modelMaxAABB.z)},
				{glm::vec3(modelMaxAABB.x, modelMinAABB.y, modelMaxAABB.z)},
				{glm::vec3(modelMinAABB.x, modelMinAABB.y, modelMaxAABB.z)},

				// Face 4 (Vertices 8,4,1,5)
				{glm::vec3(modelMinAABB.x, modelMaxAABB.y, modelMaxAABB.z)},
				{glm::vec3(modelMinAABB.x, modelMinAABB.y, modelMaxAABB.z)},
				{glm::vec3(modelMinAABB.x, modelMinAABB.y, modelMinAABB.z)},
				{glm::vec3(modelMinAABB.x, modelMaxAABB.y, modelMinAABB.z)},

				// Face 5 (Vertices 5,6,2,1)
				{glm::vec3(modelMinAABB.x, modelMaxAABB.y, modelMinAABB.z)},
				{glm::vec3(modelMaxAABB.x, modelMaxAABB.y, modelMinAABB.z)},
				{glm::vec3(modelMaxAABB.x, modelMinAABB.y, modelMinAABB.z)},
				{glm::vec3(modelMinAABB.x, modelMinAABB.y, modelMinAABB.z)}
			};

			VertexBuffer = sPtrCreate<Buffer<Vertex1P>>();
			ArrayOfVertices = sPtrCreate<VertexArray<Vertex1P>>();
			ArrayOfVertices->Bind();
			VertexBuffer->Bind();
			VertexBuffer->SetData(Vertices);
			ArrayOfVertices->SetLayout();
			ArrayOfVertices->Unbind();

			uint32_t index = 0;
			for (const auto& subMesh : mesh.Submeshes)
			{
				const auto& minAABB = subMesh.MinAABB;
				const auto& maxAABB = subMesh.MaxAABB;

				BoundBoxComponent subAABB;
				// For empty box only 5 faces are enough to draw in order to draw all edges using GL_LINE_LOOP
				subAABB.Vertices = {
					// Face 1 (Vertices 1,2,3,4)
					{glm::vec3(minAABB.x, minAABB.y, minAABB.z)},
					{glm::vec3(maxAABB.x, minAABB.y, minAABB.z)},
					{glm::vec3(maxAABB.x, minAABB.y, maxAABB.z)},
					{glm::vec3(minAABB.x, minAABB.y, maxAABB.z)},

					// Face 2 (Vertices 8,5,6,7)
					{glm::vec3(minAABB.x, maxAABB.y, maxAABB.z)},
					{glm::vec3(minAABB.x, maxAABB.y, minAABB.z)},
					{glm::vec3(maxAABB.x, maxAABB.y, minAABB.z)},
					{glm::vec3(maxAABB.x, maxAABB.y, maxAABB.z)},

					// Face 3 (Vertices 8,7,3,4)
					{glm::vec3(minAABB.x, maxAABB.y, maxAABB.z)},
					{glm::vec3(maxAABB.x, maxAABB.y, maxAABB.z)},
					{glm::vec3(maxAABB.x, minAABB.y, maxAABB.z)},
					{glm::vec3(minAABB.x, minAABB.y, maxAABB.z)},

					// Face 4 (Vertices 8,4,1,5)
					{glm::vec3(minAABB.x, maxAABB.y, maxAABB.z)},
					{glm::vec3(minAABB.x, minAABB.y, maxAABB.z)},
					{glm::vec3(minAABB.x, minAABB.y, minAABB.z)},
					{glm::vec3(minAABB.x, maxAABB.y, minAABB.z)},

					// Face 5 (Vertices 5,6,2,1)
					{glm::vec3(minAABB.x, maxAABB.y, minAABB.z)},
					{glm::vec3(maxAABB.x, maxAABB.y, minAABB.z)},
					{glm::vec3(maxAABB.x, minAABB.y, minAABB.z)},
					{glm::vec3(minAABB.x, minAABB.y, minAABB.z)}
				};

				subAABB.VertexBuffer = sPtrCreate<Buffer<Vertex1P>>();
				subAABB.ArrayOfVertices = sPtrCreate<VertexArray<Vertex1P>>();
				subAABB.ArrayOfVertices->Bind();
				subAABB.VertexBuffer->Bind();
				subAABB.VertexBuffer->SetData(subAABB.Vertices);
				subAABB.ArrayOfVertices->SetLayout();
				subAABB.ArrayOfVertices->Unbind();

				SubAABBs.push_back(subAABB);
			}
		}

		BoundBoxComponent() = default;

		std::vector<Vertex1P> Vertices;
		sPtr<Buffer<Vertex1P>> VertexBuffer;
		sPtr<VertexArray<Vertex1P>> ArrayOfVertices;
		std::vector<BoundBoxComponent> SubAABBs; // // Axis Aligned Bounding Boxes of submeshes
	};

	struct EntityTagComponent
	{
		std::string Name = "Default";
		std::string AssetFilePath = "";

		EntityTagComponent(const std::string& assetFilePath) 
			: AssetFilePath(assetFilePath) 
		{
			std::size_t foundBegin = assetFilePath.find_last_of("\\");
			std::size_t foundEnd = assetFilePath.find_last_of(".");
			std::string fileName = assetFilePath.substr(foundBegin + 1, foundEnd - foundBegin - 1);
			Name = fileName;
		}
		EntityTagComponent() = default;
		EntityTagComponent(const EntityTagComponent& other) = default;
	};
}