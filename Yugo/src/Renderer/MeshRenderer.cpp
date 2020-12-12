#include "pch.h"
#include "MeshRenderer.h"
#include "Renderer/Renderer2D.h"

namespace Yugo
{

	/**
	 * @brief Submits data to buffers and sets vertex array object (VAO).
	 *
	 * Mesh can consists of one or multiple submeshes.
	 *
	 * @param mesh Mesh component which holds buffer objects as well as VAO.
	 */
	void MeshRenderer::Submit(MeshComponent& mesh)
	{
		for (auto& submesh : mesh.Submeshes)
		{
			auto vertexArray = sPtrCreate<VertexArray<Vertex1P1N1T4J>>();
			auto vertexBuffer = sPtrCreate<Buffer<Vertex1P1N1T4J>>();
			auto indexBuffer = sPtrCreate<Buffer<unsigned int>>();

			vertexArray->Bind();
			vertexBuffer->Bind();
			indexBuffer->Bind();

			vertexBuffer->SetData(submesh.Vertices);
			indexBuffer->SetData(submesh.Indices);
			vertexArray->SetLayout();

			vertexArray->Unbind();

			submesh.ArrayOfVertices = vertexArray;
			submesh.VertexBuffer = vertexBuffer;
			submesh.IndexBuffer = indexBuffer;
		}

		//glEnable(GL_DEPTH_TEST);
	}

	/**
	 * @brief Creates vertex array object (VAO) for already submitted buffers.
	 *
	 * This method is called only when user clicks "Play Scene" and new window pops up.
	 * New window ("Game" window) has its own OpenGL context and
	 * it shares only objects that holds data, such as buffers and shaders, with main window's OpenGL context.
	 *
	 * @param mesh Mesh component which holds buffer objects as well as VAO.
	 */
	void MeshRenderer::SubmitCopy(MeshComponent& mesh)
	{
		for (auto& submesh : mesh.Submeshes)
		{
			submesh.ArrayOfVertices.reset();

			auto vertexArray = sPtrCreate<VertexArray<Vertex1P1N1T4J>>();

			vertexArray->Bind();
			submesh.VertexBuffer->Bind();
			submesh.IndexBuffer->Bind();
			vertexArray->SetLayout();
			vertexArray->Unbind();

			submesh.ArrayOfVertices = vertexArray;
		}

		//glEnable(GL_DEPTH_TEST);
	}

	/**
	 * @brief Renders mesh.
	 *
	 * This method renders mesh on the screen.
	 * Mesh can holds animation data (verticies affected by animation and weights that define by how much a vertex is affected).
	 * Textures are also bound and samplers are set.
	 *
	 * @param mesh Mesh component that holds buffers data for rendering.
	 * @param transform Transform component that holds transformation data (position, rotation, scale) about mesh.
	 * @param camera Camera component that holds camera position, projection matrix, view matrix and more.
	 * @param shader Shader that will be used for rendering.
	 */
	void MeshRenderer::Render(const MeshComponent& mesh, const TransformComponent& transform, const CameraComponent& camera, const Shader& shader)
	{
		shader.Use();

		auto& modelMat = transform.ModelMatrix;
		auto& viewMat = camera.View;
		auto& projectionMat = camera.Projection;

		shader.SetMat4("model", modelMat);
		shader.SetMat4("view", viewMat);
		shader.SetMat4("projection", projectionMat);

		if (mesh.HasAnimation)
			for (uint32_t i = 0; i < mesh.FinalTransforms.size(); ++i)
				shader.SetMat4("finalTransforms[" + std::to_string(i) + "]", mesh.FinalTransforms[i]);

		for (const auto& submesh : mesh.Submeshes)
		{
			// Nr stands for number of diffuse/specular/.... textures in one mesh
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;

			for (unsigned int i = 0; i < submesh.Textures.size(); ++i)
			{
				glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
				std::string number;

				const std::string& name = submesh.Textures[i].GetType();

				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);
				else if (name == "texture_normal")
					number = std::to_string(normalNr++);
				else if (name == "texture_height")
					number = std::to_string(heightNr++);

				// now set the sampler to the correct texture unit
				shader.SetInt("material." + name + number, i);
				submesh.Textures[i].Bind();
			}

			submesh.ArrayOfVertices->Bind();
			glDrawElements(GL_TRIANGLES, submesh.Indices.size(), GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, submesh.Vertices.size());
			submesh.ArrayOfVertices->Unbind();
		}
	}

	/**
	 * @brief Renders Axis Aligned Bounding Box (AABB).
	 *
	 * This method renders AABB for debug purpose
	 *
	 * @param aabb Box component that holds buffers data for cube rendering.
	 * @param transform Transform component that holds transformation data (position, rotation, scale) about mesh.
	 * @param camera Camera component that holds camera position, projection matrix, view matrix and more.
	 * @param shader Shader that will be used for rendering.
	 */
	void MeshRenderer::DrawAABB(const BoundBoxComponent& aabb, const TransformComponent& transform, const CameraComponent& camera, const Shader& shader)
	{
		shader.Use();

		auto& modelMat = transform.ModelMatrix;
		auto& viewMat = camera.View;
		auto& projectionMat = camera.Projection;

		shader.SetMat4("model", modelMat);
		shader.SetMat4("view", viewMat);
		shader.SetMat4("projection", projectionMat);

		aabb.ArrayOfVertices->Bind();
		glDrawArrays(GL_LINE_LOOP, 0, 20);
		aabb.ArrayOfVertices->Unbind();
	}

	//void MeshRenderer::EnableBlend()
	//{
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//}

	//void MeshRenderer::DisableBlend()
	//{
	//	glDisable(GL_BLEND);
	//}

	//void MeshRenderer::EnableDepthTest()
	//{
	//	glEnable(GL_DEPTH_TEST);
	//}

	//void MeshRenderer::DisableDepthTest()
	//{
	//	glDisable(GL_DEPTH_TEST);
	//}

	//void MeshRenderer::ClearColor(float r, float g, float b)
	//{
	//	glClearColor(r, g, b, 1.0f);
	//}

	//void MeshRenderer::ClearColorBuffer()
	//{
	//	glClear(GL_COLOR_BUFFER_BIT);
	//}

	//void MeshRenderer::ClearDepthBuffer()
	//{
	//	glClear(GL_DEPTH_BUFFER_BIT);
	//}


}