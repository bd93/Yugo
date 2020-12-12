#include "pch.h"
#include "SpriteRenderer.h"


namespace Yugo
{

	void SpriteRenderer::Submit(SpriteComponent& sprite)
	{
		auto vertexArray = sPtrCreate<VertexArray<Vertex1P1T>>();
		auto vertexBuffer = sPtrCreate<Buffer<Vertex1P1T>>();
		auto indexBuffer = sPtrCreate<Buffer<uint32_t>>();

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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		glDisable(GL_BLEND);
	}

	void SpriteRenderer::RenderInstanced(const SpriteComponent& sprite, size_t numOfInstances, const CameraComponent& camera, const Shader& shader)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		shader.Use();

		auto& projectionMat = camera.Projection;
		auto& viewMat = camera.View;

		shader.SetMat4("projection", projectionMat);
		shader.SetMat4("view", viewMat);
		shader.SetVec4("color", sprite.Color);
		shader.SetInt("texture1", 0); // temp
		glActiveTexture(GL_TEXTURE0);
		sprite.Texture.Bind();

		sprite.ArrayOfVertices->Bind();
		glDrawElementsInstanced(GL_TRIANGLES, sprite.Indices.size(), GL_UNSIGNED_INT, 0, numOfInstances);
		sprite.ArrayOfVertices->Unbind();

		glDisable(GL_BLEND);
	}

}

