#include "pch.h"
#include "SpriteRenderer.h"


namespace Yugo
{

	void SpriteRenderer::Submit(SpriteComponent& sprite)
	{
		auto& vertexArray = sPtrCreate<VertexArray<Vertex1P1T>>();
		auto& vertexBuffer = sPtrCreate<Buffer<Vertex1P1T>>();
		auto& indexBuffer = sPtrCreate<Buffer<uint32_t>>();

		vertexArray->Bind();
		vertexBuffer->Bind();
		indexBuffer->Bind();

		vertexBuffer->SetData(sprite.Vertices);
		indexBuffer->SetData(sprite.Indices);
		vertexArray->SetLayout();

		vertexArray->Unbind();

		sprite.ArrayOfVertices = vertexArray;
		sprite.VertexBuffer = vertexBuffer;
		sprite.IndexBuffer = indexBuffer;
	}

	void SpriteRenderer::Render(const SpriteComponent& sprite, const TransformComponent& transform, const CameraComponent& camera, const Shader& shader)
	{
		EnableBlend();

		shader.Use();

		auto& modelMat = transform.ModelMatrix;
		auto& projectionMat = camera.Projection;

		shader.SetMat4("model", modelMat);
		shader.SetMat4("projection", projectionMat);
		shader.SetVec4("color", sprite.Color);

		glActiveTexture(GL_TEXTURE0);
		sprite.Texture.Bind();

		sprite.ArrayOfVertices->Bind();
		glDrawElements(GL_TRIANGLES, sprite.Indices.size(), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, sprite.Vertices.size());
		sprite.ArrayOfVertices->Unbind();

		DisableBlend();
	}

	void SpriteRenderer::EnableBlend()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void SpriteRenderer::DisableBlend()
	{
		glDisable(GL_BLEND);
	}

	void SpriteRenderer::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void SpriteRenderer::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void SpriteRenderer::ClearColorBuffer(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void SpriteRenderer::ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

}

