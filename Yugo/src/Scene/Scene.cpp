#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Core/FileSystem.h"
#include "Core/ModelImporter.h"
#include "Renderer/Renderer2D.h"
#include "Events/ApplicationEvent.h"
#include "Animation/Animation.h"
#include "Serializer/Serializer.h"
#include "Scripting/ScriptComponent.h"
#include "Renderer/SpriteRenderer.h"
#include "UI/UserInterface.h"
#include "Renderer/TextRenderer.h"


namespace Yugo
{

	Scene::Scene()
	{
		m_Camera = sPtrCreate<Camera>(glm::vec3(100.0f, 100.0f, 100.0f));
	}

	void Scene::OnStart()
	{
		Shader modelShader(
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\model_vertex.shader",
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\model_fragment.shader"
		);
		ResourceManager::AddShader("modelShader", modelShader);

		Shader animatedModelShader (
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\vertexAnimation.shader",
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\model_fragment.shader"
			);
		ResourceManager::AddShader("animatedModelShader", animatedModelShader);

		m_Camera->OnStart();

		MeshRenderer::SetCamera(m_Camera);
		Renderer2D::SetCamera(m_Camera);
	}

	void Scene::OnEvent(const Event& event)
	{
		if (event.GetEventType() == EventType::MouseButtonPress)
		{

		}
	}

	void Scene::OnUpdate(float ts)
	{
		m_Camera->OnUpdate(ts);

		auto view = m_Registry.view<TransformComponent>(entt::exclude<SpriteComponent>);
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);

			transform.ModelMatrix = glm::mat4(1.0f);
			transform.ModelMatrix = glm::translate(transform.ModelMatrix, transform.Position);
			transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			transform.ModelMatrix = glm::scale(transform.ModelMatrix, transform.Scale);
		}
	}

	void Scene::OnRender()
	{
		MeshRenderer::ClearColorBuffer(0.3f, 0.3f, 0.3f);
		MeshRenderer::ClearDepthBuffer();

		Renderer2D::DrawGrid(glm::vec3(0.0f, 0.0f, 0.0f), 100, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.2f, 0.2f, 0.2f));

		MeshRenderer::ClearDepthBuffer(); // TODO: Check how to fix this double call
		
		// Render meshes with animations
		{
			auto view = m_Registry.view<MeshComponent, TransformComponent, AnimationComponent>();
			for (auto entity : view)
			{
				auto& [mesh, transform, animation] = view.get<MeshComponent, TransformComponent, AnimationComponent>(entity);
				if (animation.IsAnimationRunning)
					MeshRenderer::Render(mesh, transform, ResourceManager::GetShader("animatedModelShader"));
				else
					MeshRenderer::Render(mesh, transform, ResourceManager::GetShader("modelShader"));
			}
		}

		// Render meshes without animations
		{
			auto view = m_Registry.view<MeshComponent, TransformComponent>(entt::exclude<AnimationComponent>);
			for (auto entity : view)
			{
				auto& [mesh, transform] = view.get<MeshComponent, TransformComponent>(entity);
				MeshRenderer::Render(mesh, transform, ResourceManager::GetShader("modelShader"));
			}
		}
	}

	void Scene::OnShutdown()
	{
	}

	void Scene::SaveScene(const std::string& filePath)
	{
		nlohmann::json jsonData;

		auto view = m_Registry.view<EntityTagComponent, TransformComponent>();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& entityTag = view.get<EntityTagComponent>(entity);
			Serializer::Serialize(entityTag, transform, jsonData);
		}

		std::ofstream out(filePath);
		out << std::setw(4) << jsonData;
	}

	void Scene::LoadScene(const std::string& filePath)
	{
		m_Registry.clear();
		
		std::ifstream in(filePath);
		nlohmann::json jsonData;
		in >> jsonData;

		for (auto& entity : jsonData["Transform"])
		{
			const auto& name = entity["EntityTag"]["Name"];
			const auto& assetFilePath = entity["EntityTag"]["AssetFilePath"];
		
			auto& [loadedMesh, loadedAnimation] = ModelImporter::LoadMeshFile(assetFilePath);

			auto meshEntity = CreateEntity();

			auto& entityTag = meshEntity.AddComponent<EntityTagComponent>(assetFilePath);
			auto& mesh = meshEntity.AddComponent<MeshComponent>(*loadedMesh);
			auto& transform = meshEntity.AddComponent<TransformComponent>();
			meshEntity.AddComponent<BoundBoxComponent>(*loadedMesh);
			if (loadedMesh->HasAnimation)
				auto& animation = meshEntity.AddComponent<AnimationComponent>(*loadedAnimation);
			
			transform.Position = glm::vec3(entity["Position"]["x"], entity["Position"]["y"], entity["Position"]["z"]);
			transform.Rotation = glm::vec3(entity["Rotation"]["x"], entity["Rotation"]["y"], entity["Rotation"]["z"]);
			transform.Scale = glm::vec3(entity["Scale"]["x"], entity["Scale"]["y"], entity["Scale"]["z"]);

			transform.ModelMatrix = glm::mat4(1.0f);
			transform.ModelMatrix = glm::translate(transform.ModelMatrix, transform.Position);
			transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			transform.ModelMatrix = glm::scale(transform.ModelMatrix, transform.Scale);

			MeshRenderer::Submit(mesh);
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = {m_Registry.create(), name, this };
		return entity;
	}

	// TEMPORARY!!!
	entt::registry& Scene::GetRegistry()
	{
		return m_Registry;
	}

}