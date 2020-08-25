#include "pch.h"
#include "Renderer2D.h"
#include "Core/ResourceManager.h"


namespace Yugo
{

	static sPtr<Camera> s_Camera;

	static Line s_Line;
	static Quad s_Quad;
	static EmptyQuad s_EmptyQuad;
	static Triangle s_Triangle;

	void Renderer2D::InitLine()
	{
		s_Line.VertexArray = sPtrCreate<VertexArray<Vertex1P>>();
		s_Line.VertexBuffer = sPtrCreate<Buffer<Vertex1P>>();

		s_Line.VertexArray->Bind();
		s_Line.VertexBuffer->Bind();

		s_Line.VertexBuffer->SetData(s_Line.Vertices);
		s_Line.VertexArray->SetLayout();

		s_Line.VertexArray->Unbind();
	}

	void Renderer2D::InitQuad()
	{
		s_Quad.VertexArray = sPtrCreate<VertexArray<Vertex1P1T>>();
		s_Quad.VertexBuffer = sPtrCreate<Buffer<Vertex1P1T>>();
		  
		s_Quad.VertexArray->Bind();
		s_Quad.VertexBuffer->Bind();
		  
		s_Quad.VertexBuffer->SetData(s_Quad.Vertices);
		s_Quad.VertexArray->SetLayout();
		  
		s_Quad.VertexArray->Unbind();
	}

	void Renderer2D::InitCircle()
	{
		// TODO
	}

	void Renderer2D::InitEmptyQuad()
	{
		s_Line.VertexArray = sPtrCreate<VertexArray<Vertex1P>>();
		s_Line.VertexBuffer = sPtrCreate<Buffer<Vertex1P>>();

		s_Line.VertexArray->Bind();
		s_Line.VertexBuffer->Bind();

		s_Line.VertexBuffer->SetData(s_EmptyQuad.Vertices);
		s_Line.VertexArray->SetLayout();

		s_Line.VertexArray->Unbind();
	}

	void Renderer2D::Init()
	{
		Shader quadShader(
			FileSystem::GetSolutionFolderPath() + "Playground\\src\\Assets\\Shaders\\vertex2d.shader",
			FileSystem::GetSolutionFolderPath() + "Playground\\src\\Assets\\Shaders\\fragment2d.shader"
		);

		ResourceManager::AddShader("quadShader", quadShader);

		Shader texturedQuadShader(
			FileSystem::GetSolutionFolderPath() + "Playground\\src\\Assets\\Shaders\\vertex2d_textured.shader",
			FileSystem::GetSolutionFolderPath() + "Playground\\src\\Assets\\Shaders\\fragment2d_textured.shader"
		);

		ResourceManager::AddShader("texturedQuadShader", texturedQuadShader);

		Texture texture(FileSystem::GetSolutionFolderPath() + "Playground\\src\\Assets\\Textures\\awesomeface.png");

		ResourceManager::AddTexture("awesomeface", texture);

		InitLine();
		InitQuad();
		InitEmptyQuad();
	}

	void Renderer2D::SetCamera(const sPtr<Camera>& camera)
	{
		s_Camera = camera;
	}

	void Renderer2D::ClearColorBuffer(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer2D::EnableBlend()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer2D::DisableBlend()
	{
		glDisable(GL_BLEND);
	}

	// Flat colored shape
	void Renderer2D::Draw(
		Shape2D shape, 
		const glm::vec3& position, 
		const glm::vec3& size, 
		const glm::vec3& color
	)
	{
		auto& shader = ResourceManager::GetShader("quadShader");
		shader.Use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, size);

		glm::mat4 view = s_Camera->GetViewMatrix();
		//glm::mat4 view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection = s_Camera->GetProjectionMatrix();
		//glm::mat4 projection = glm::ortho(-600.0f, +600.0f, -400.0f, 400.0f, 0.1f, 100.0f);

		shader.SetMat4("model", model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		glm::vec3 shapeColor = color;
		shader.SetVec3("color", shapeColor);

		if (shape == Shape2D::Line)
		{
			s_Line.VertexArray->Bind();
			glDrawArrays(GL_LINES, 0, 2);
			s_Line.VertexArray->Unbind();
		}
		if (shape == Shape2D::Quad)
		{
			s_Quad.VertexArray->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			s_Quad.VertexArray->Unbind();
		}

	}

	//Textured shape (quad)
	void Renderer2D::Draw(
		Shape2D shape, const 
		glm::vec3& position, 
		const glm::vec3& size, 
		const Texture& texture, 
		const glm::vec3& color
	)
	{
		auto& shader = ResourceManager::GetShader("texturedQuadShader");
		shader.Use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, size);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection = glm::ortho(-600.0f, +600.0f, -400.0f, 400.0f, 0.1f, 100.0f);

		shader.SetMat4("model", model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		glm::vec3 shapeColor = color;
		shader.SetVec3("color", shapeColor);

		EnableBlend();
		if (shape == Shape2D::Quad)
		{
			texture.Bind();
			s_Quad.VertexArray->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			s_Quad.VertexArray->Unbind();
		}
		DisableBlend();
	}

	// Rotated flat colored shape
	void Renderer2D::DrawRotated(
		Shape2D shape, 
		const glm::vec3& position, 
		const glm::vec3& size, 
		const glm::vec3& rotation, 
		const glm::vec3& color
	)
	{
		auto& shader = ResourceManager::GetShader("quadShader");
		shader.Use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, size);

		glm::mat4 view = s_Camera->GetViewMatrix();
		//glm::mat4 view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection = s_Camera->GetProjectionMatrix();
		//glm::mat4 projection = glm::ortho(-600.0f, +600.0f, -400.0f, 400.0f, 0.1f, 100.0f);

		shader.SetMat4("model", model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		glm::vec3 shapeColor = color;
		shader.SetVec3("color", shapeColor);

		if (shape == Shape2D::Line)
		{
			s_Line.VertexArray->Bind();
			glDrawArrays(GL_LINES, 0, 2);
			s_Line.VertexArray->Unbind();
		}
		if (shape == Shape2D::Quad)
		{
			s_Quad.VertexArray->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			s_Quad.VertexArray->Unbind();
		}
	}

	// Rotated textured shape (quad)
	void Renderer2D::DrawRotated(
		Shape2D shape, 
		const glm::vec3& position, 
		const glm::vec3& size, 
		const glm::vec3& rotation,
		const Texture& texture, 
		const glm::vec3& color
	)
	{
		auto& shader = ResourceManager::GetShader("texturedQuadShader");
		shader.Use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, size);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection = glm::ortho(-600.0f, +600.0f, -400.0f, 400.0f, 0.1f, 100.0f);

		shader.SetMat4("model", model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		glm::vec3 shapeColor = color;
		shader.SetVec3("color", shapeColor);

		EnableBlend();
		if (shape == Shape2D::Quad)
		{
			texture.Bind();
			s_Quad.VertexArray->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			s_Quad.VertexArray->Unbind();
		}
		DisableBlend();
	}

	// Instanced flat colored shapes
	void Renderer2D::DrawInstanced(
		Shape2D shape,
		const glm::vec3& position, 
		const glm::vec3& size, 
		const std::vector<glm::mat4>& models, 
		const glm::vec3& color
	)
	{
		// TODO: Implement
	}

	// Instanced textured shapes
	void Renderer2D::DrawInstanced(
		Shape2D shape,
		const glm::vec3& position, 
		const glm::vec3& size, 
		const Texture& texture, 
		const std::vector<glm::mat4>& models, 
		const glm::vec3& color
	)
	{
		// TODO: Implement
	}

	void Renderer2D::DrawEmptyQuad(
		const glm::vec3& position,
		const glm::vec3& size,
		const glm::vec3& color // Color of a line
	)
	{
		auto& shader = ResourceManager::GetShader("quadShader");
		shader.Use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, size);

		glm::mat4 view = s_Camera->GetViewMatrix();
		glm::mat4 projection = s_Camera->GetProjectionMatrix();

		shader.SetMat4("model", model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		glm::vec3 shapeColor = color;
		shader.SetVec3("color", shapeColor);

		s_Line.VertexArray->Bind();
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		s_Line.VertexArray->Unbind();
	}

	void Renderer2D::DrawRotatedEmptyQuad(
		const glm::vec3& position, 
		const glm::vec3& size, 
		const glm::vec3& rotation,
		const glm::vec3& color // Color of a line
	)
	{
		auto& shader = ResourceManager::GetShader("quadShader");
		shader.Use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, size);

		glm::mat4 view = s_Camera->GetViewMatrix();
		glm::mat4 projection = s_Camera->GetProjectionMatrix();

		shader.SetMat4("model", model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		glm::vec3 shapeColor = color;
		shader.SetVec3("color", shapeColor);

		s_Line.VertexArray->Bind();
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		s_Line.VertexArray->Unbind();
	}

	/*
	This grid is on XZ plane;
	*/
	void Renderer2D::DrawGrid(
		const glm::vec3& position,
		uint32_t numOfQuads,
		const glm::vec3& quadSize,
		const glm::vec3& color
	)
	{
		uint32_t numOfQuadsPerEdge = (uint32_t)sqrt(numOfQuads);
		assert((numOfQuadsPerEdge * numOfQuadsPerEdge) == numOfQuads); // Number of rows and columns must be the same;
		float xCoord = position.x - (numOfQuadsPerEdge / 2) * quadSize.x;
		for (uint32_t i = 0; i < numOfQuadsPerEdge; ++i)
		{
			float zCoord = position.z + ((numOfQuadsPerEdge / 2) * quadSize.z);
			for (uint32_t j = 0; j < numOfQuadsPerEdge; ++j)
			{
				DrawRotatedEmptyQuad(glm::vec3(xCoord, 0.0f, zCoord), quadSize, glm::vec3(-90.0f, 0.0f, 0.0f), color);
				zCoord -= quadSize.z;
			}
			xCoord += quadSize.x;
		}
	}

	void Renderer2D::DrawRotatedGrid(
		const glm::vec3& position, 
		uint32_t numOfQuads, 
		const glm::vec3& quadSize, 
		const glm::vec3& rotation,
		const glm::vec3& color
	)
	{
		// TODO: Implement
	}

}