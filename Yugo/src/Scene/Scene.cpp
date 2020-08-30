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


namespace Yugo
{

	Scene::Scene()
		: m_SelectedEntity(entt::null)
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
		// Import asset event is invoked when user drag and drop asset to scene imgui window 
		if (event.GetEventType() == EventType::ImportAsset)
		{
			const auto& importAssetEvent = static_cast<const ImportAssetEvent&>(event);
			const auto& importAssetFilePath = importAssetEvent.GetAssetFilePath();
			auto& [loadedMesh, loadedAnimation] = ModelImporter::LoadMeshFile(importAssetFilePath);

			auto meshEntity = CreateEntity();

			auto& entityTag = meshEntity.AddComponent<EntityTagComponent>(importAssetFilePath);
			auto& mesh = meshEntity.AddComponent<MeshComponent>(*loadedMesh);
			auto& transform = meshEntity.AddComponent<TransformComponent>();
			meshEntity.AddComponent<BoundBoxComponent>(*loadedMesh);
			if (loadedMesh->HasAnimation)	
				auto& animation = meshEntity.AddComponent<AnimationComponent>(*loadedAnimation);

			transform.ModelMatrix = glm::mat4(1.0f);

			// Check if mouse ray intersects ground plane
			if (m_MouseRay.CheckCollisionWithPlane())
			{
				// Place asset on the intersection point
				const glm::vec3& intersectionPoint = m_MouseRay.GetIntersectionPoint();
				transform.ModelMatrix = glm::translate(transform.ModelMatrix, intersectionPoint);
				transform.Position = intersectionPoint;
			}
			else
			{
				// Place asset on the origin point of world coordinate system
				transform.ModelMatrix = glm::translate(transform.ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
				transform.Position = glm::vec3(0.0f, 0.0f, 0.0f);
			}

			transform.ModelMatrix = glm::scale(transform.ModelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
			transform.Scale = glm::vec3(10.0f, 10.0f, 10.0f);
			
			MeshRenderer::Submit(mesh);
		}

		if (event.GetEventType() == EventType::MouseButtonPress)
		{
			auto view = m_Registry.view<MeshComponent, TransformComponent, BoundBoxComponent>();

			bool isAnyMeshSelected = false;
			int intersectionDistance = -1; // Initialize with -1 which means no mesh is selected
			for (auto entity : view)
			{
				const auto& [mesh, transform] = view.get<MeshComponent, TransformComponent>(entity);
				
				// Check if mouse ray intersects mesh (model)
				if (m_MouseRay.CheckCollisionWithBox(mesh, transform))
				{
					isAnyMeshSelected = true;
					if (intersectionDistance == -1)
					{
						// Check if mouse ray intersects any triangle in mesh
						if (m_MouseRay.CheckCollisionWithMesh(mesh, transform))
						{
							intersectionDistance = (int)m_MouseRay.GetCollisionDistance();
							m_SelectedEntity = entity;
						}
					}
					// In case mesh is behind slected mesh
					if (intersectionDistance < (int)m_MouseRay.GetCollisionDistance())
						continue;
					if (intersectionDistance > (int)m_MouseRay.GetCollisionDistance())
					{
						// Check if mouse ray intersects any triangle in mesh
						if (m_MouseRay.CheckCollisionWithMesh(mesh, transform))
						{
							intersectionDistance = (int)m_MouseRay.GetCollisionDistance();
							m_SelectedEntity = entity;
						}
					}
				}
			}
			if (!isAnyMeshSelected)
				m_SelectedEntity = entt::null;
		}

		if (event.GetEventType() == EventType::MouseScroll)
			m_Camera->OnEvent(event);
	}

	void Scene::OnUpdate(float ts)
	{
		m_Camera->OnUpdate(ts);

		{
			auto view = m_Registry.view<MeshComponent, TransformComponent, AnimationComponent>();
			for (auto entity : view)
			{
				auto& [mesh, animation] = view.get<MeshComponent, AnimationComponent>(entity);
				if (animation.IsAnimationRunning)
					Animation::RunAnimation(mesh, animation);
			}
		}

		//{
		//	auto view = m_Registry.view<TransformComponent>();
		//	for (auto entity : view)
		//	{
		//		auto& transform = view.get<TransformComponent>(entity);

		//		transform.ModelMatrix = glm::mat4(1.0f);
		//		transform.ModelMatrix = glm::translate(transform.ModelMatrix, transform.Position);
		//		transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		//		transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		//		transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		//		transform.ModelMatrix = glm::scale(transform.ModelMatrix, transform.Scale);
		//	}
		//}
	}

	void Scene::OnRender()
	{
		MeshRenderer::ClearColorBuffer(0.3f, 0.3f, 0.3f);
		MeshRenderer::ClearDepthBuffer();

		Renderer2D::DrawGrid(glm::vec3(0.0f, 0.0f, 0.0f), 100, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.2f, 0.2f, 0.2f));

		MeshRenderer::ClearDepthBuffer(); // TODO: Check how to fix this double call

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

		{
			auto view = m_Registry.view<MeshComponent, TransformComponent>(entt::exclude<AnimationComponent>);
			for (auto entity : view)
			{
				auto& [mesh, transform] = view.get<MeshComponent, TransformComponent>(entity);
				MeshRenderer::Render(mesh, transform, ResourceManager::GetShader("modelShader"));
			}
		}

		{
			auto view = m_Registry.view<BoundBoxComponent, TransformComponent>();
			for (auto entity : view)
			{
				auto& [aabb, transform] = view.get<BoundBoxComponent, TransformComponent>(entity);
				MeshRenderer::DrawAABB(aabb, transform, ResourceManager::GetShader("quadShader"));

				//for (const auto& subAABB : aabb.SubAABBs)
				//{
				//	MeshRenderer::DrawAABB(subAABB, transform, ResourceManager::GetShader("quadShader"));
				//}
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

	void Scene::CastMouseRay(float mousePosX, float mousePosY, int sceneWidth, int sceneHeight)
	{
		m_MouseRay.CalculateRayOrigin(m_Camera, mousePosX, mousePosY, sceneWidth, sceneHeight);
	}

	bool Scene::IsAnyMeshSelected()
	{
		if (m_SelectedEntity == entt::null)
			return false;
		else
			return true;
	}

	TransformComponent& Scene::GetSelectedMeshTransform()
	{
		auto view = m_Registry.view<MeshComponent, TransformComponent, BoundBoxComponent>();

		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			if (entity == m_SelectedEntity)
				return transform;
		}
	}

	// TEMPORARY!!!
	entt::registry& Scene::GetRegistry()
	{
		return m_Registry;
	}

}