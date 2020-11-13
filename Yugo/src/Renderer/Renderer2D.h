#pragma once
#include "Vertex.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Core/Core.h"
#include "Shader.h"
#include "Core/FileSystem.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera/Camera.h"

#include <glm/glm.hpp>



namespace Yugo
{

	enum class Shape2D
	{
		Line,
		Triangle,
		Quad,
		Circle
	};

	struct Line
	{
		std::vector<Vertex1P> Vertices {
			{glm::vec3(0.0f, 0.0f, 0.0f)},
			{glm::vec3(1.0f, 0.0f, 0.0f)}
		};

		sPtr<Buffer<Vertex1P>> VertexBuffer;
		sPtr<VertexArray<Vertex1P>> VertexArray;
	};

	struct Triangle
	{
		std::vector<Vertex1P1T> Vertices{
			{glm::vec3(0.0f,  0.5f, 0.0f), glm::vec2(0.5f, 1.0f)},
			{glm::vec3(0.5f, -0.42f, 0.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(-0.5f,-0.42f, 0.0f), glm::vec2(0.0f, 0.0f)}
		};

		sPtr<Buffer<Vertex1P1T>> VertexBuffer;
		sPtr<VertexArray<Vertex1P1T>> VertexArray;
	};

	struct Quad
	{
		std::vector<Vertex1P1T> Vertices {
			{glm::vec3(1.0f,  0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
			{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.0f,  0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
			{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
			{glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
			{glm::vec3(0.0f,  0.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
		};

		sPtr<Buffer<Vertex1P1T>> VertexBuffer;
		sPtr<VertexArray<Vertex1P1T>> VertexArray;
	};

	/*
	Empty quad (without color or texture);
	This quad doesn't have middle line from 2 triangles, which is the case with struct Quad,
	because that quad is drawn as 2 triangles that share the same edge
	*/ 
	struct EmptyQuad
	{
		std::vector<Vertex1P> Vertices{
			{glm::vec3(1.0f,  0.0f, 0.0f)},
			{glm::vec3(1.0f, -1.0f, 0.0f)},
			{glm::vec3(0.0f, -1.0f, 0.0f)},
			{glm::vec3(0.0f,  0.0f, 0.0f)}
		};

		sPtr<Buffer<Vertex1P>> VertexBuffer;
		sPtr<VertexArray<Vertex1P>> VertexArray;
	};

	struct Circle
	{
		// TODO: Implement circle shape
	};

	class Renderer2D
	{
	private:
		static void InitLine();
		static void InitQuad();
		static void InitCircle();
		static void InitEmptyQuad();
		
		static void EnableBlend();
		static void DisableBlend();

	public:
		static void OnStart();
		//static void SetCamera(const sPtr<Camera>& camera);
		// r = red, g = green, b = blue
		static void ClearColorBuffer(float r, float g, float b);

		static void Draw(
			Shape2D shape,
			const CameraComponent& camera,
			const glm::vec3& position,
			const glm::vec3& size,
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		static void Draw(
			Shape2D shape,
			const glm::vec3& position,
			const glm::vec3& size,
			const Texture& texture,
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		static void DrawRotated(
			Shape2D shape,
			const CameraComponent& camera,
			const glm::vec3& position,
			const glm::vec3& size,
			const glm::vec3& rotation,
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		static void DrawRotated(
			Shape2D shape,
			const glm::vec3& position,
			const glm::vec3& size,
			const glm::vec3& rotation,
			const Texture& texture,
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)
		);

		static void DrawInstanced(
			Shape2D shape,
			const CameraComponent& camera,
			const glm::vec3& position, // Position of the whole geometry that comtains instanced shapes
			const glm::vec3& size, // Size of single shape
			const std::vector<glm::mat4>& models, // Transformation matrices for instanced shapes 
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		static void DrawInstanced(
			Shape2D shape,
			const CameraComponent& camera,
			const glm::vec3& position, // Position of the whole geometry that comtains instanced shapes
			const glm::vec3& size, // Size of single shape
			const Texture& texture,
			const std::vector<glm::mat4>& models, // Transformation matrices for instanced shapes 
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)
		);

		// Draw empty quad with 4 Line shapes
		static void DrawEmptyQuad(
			const CameraComponent& camera,
			const glm::vec3& position, 
			const glm::vec3& size,
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		static void DrawRotatedEmptyQuad(
			const CameraComponent& camera,
			const glm::vec3& position,
			const glm::vec3& size,
			const glm::vec3& rotation,
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)
		);

		// Draw grid that consists of empty quads
		static void DrawGrid(
			const CameraComponent& camera,
			const glm::vec3& position,
			uint32_t numOfQuads,
			const glm::vec3& quadSize,
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)
		);
		static void DrawRotatedGrid(
			const CameraComponent& camera,
			const glm::vec3& position,
			uint32_t numOfQuads,
			const glm::vec3& quadSize,
			const glm::vec3& rotation,
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)
		);
	};

}