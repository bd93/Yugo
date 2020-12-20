#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Core/FileSystem.h"
#include "Core/ModelImporter.h"
#include "Renderer/Renderer2D.h"
#include "Events/ApplicationEvent.h"
#include "Animation/Animation.h"
#include "Serializer/Serializer.h"
#include "Scripting/Components.h"
#include "Renderer/SpriteRenderer.h"
#include "GameUI/UserInterface.h"
#include "Renderer/TextRenderer.h"


namespace Yugo
{

	Scene::Scene()
	{
		//m_UserInterface = uPtrCreate<UserInterface>(this);
	}

	/**
	 * @brief Method to be called during application OnStart stage.
     *
     * This method adds necessary assets to ResourceManager's unordered maps.
     * ResourceManager can be used anywhere in code in order to retreive assets.
     * The idea is to load necessary assets only once at the beginning of app and use it later.
	 * It also initialize camera and game UI.
     */
	void Scene::OnStart()
	{
#ifdef YU_RELEASE
		Dispatcher::Subscribe<KeyboardKeyPress>(this);
		Dispatcher::Subscribe<MouseButtonPress>(this);
		Dispatcher::Subscribe<MouseScroll>(this);
		Dispatcher::Subscribe<ImportAssetEvent>(this);
#endif

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

		Shader instancedRendering(
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\instancedRenderingVertex.shader",
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\instancedRenderingFragment.shader"
			);
		ResourceManager::AddShader("instancedRendering", instancedRendering);

		//m_UserInterface->OnStart();

		auto cameraEntity = CreateEntity();
		auto& transform = cameraEntity.AddComponent<TransformComponent>();
		auto& camera = cameraEntity.AddComponent<CameraComponent>();
		auto& tag = cameraEntity.AddComponent<EntityTagComponent>();
		tag.Name = "Main Camera";
		Camera::OnStart(transform, camera);

		m_Grid = sPtrCreate<Grid>(65536, 50.0f, 2.2f);
	}

	/**
     * @brief Method to be called during application OnEvent stage.
     *
     * This method deals with events, such as mouse click, button press, etc.
	 *
	 * @param event Base class for all types of events.
     */
	void Scene::OnEvent(const Event& event)
	{
		// TODO: Update new camera Projection matrix when WindowResize event occurs
	}

	/**
     * @brief Method to be called during application OnUpdate stage.
     *
     * This method updates camera and transform components each frame.
	 *
	 * @param timeStep Duration of each frame.
     */
	void Scene::OnUpdate(TimeStep ts)
	{
		// Update camera
		{
			auto view = m_Registry.view<CameraComponent, TransformComponent, EntityTagComponent>();
			for (auto entity : view)
			{
				auto& [camera, transform, tag] = view.get<CameraComponent, TransformComponent, EntityTagComponent>(entity);
				if (tag.Name == "Main Camera")
					Camera::Move(ts, transform, camera);
			}
		}
		// Update entity
		{
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
		// Update game UI
		//m_UserInterface->OnUpdate(ts);
	}

	/**
     * @brief Method to be called during application OnRender stage.
     *
     * This method updates camera and transform components each frame.
     */
	void Scene::OnRender()
	{
		auto& camera = GetCamera();

		// Render meshes with animations
		{
			auto view = m_Registry.view<MeshComponent, TransformComponent, AnimationComponent>();
			for (auto entity : view)
			{
				auto& [mesh, transform, animation] = view.get<MeshComponent, TransformComponent, AnimationComponent>(entity);
				if (animation.IsAnimationRunning)
					MeshRenderer::Render(mesh, transform, camera, ResourceManager::GetShader("animatedModelShader"));
				else
					MeshRenderer::Render(mesh, transform, camera, ResourceManager::GetShader("modelShader"));
			}
		}

		// Render meshes without animations
		{
			auto view = m_Registry.view<MeshComponent, TransformComponent>(entt::exclude<AnimationComponent>);
			for (auto entity : view)
			{
				auto& [mesh, transform] = view.get<MeshComponent, TransformComponent>(entity);
				MeshRenderer::Render(mesh, transform, camera, ResourceManager::GetShader("modelShader"));
			}
		}

		//m_Grid->Draw(camera, ResourceManager::GetShader("instancedRendering"));

		//m_UserInterface->OnRender();
	}

	/**
     * @brief Method to be called during application OnShutdown stage.
     *
     * @note Currently there is nothing to put here.
     */
	void Scene::OnShutdown()
	{
		//m_UserInterface->OnShutdown();
		// TODO: Scene shutdown goes here...
	}

	/**
	 * @brief Save scene to json file.
	 *
	 * Serialize components to json file and keep track of entities which these components are attached to.
	 *
	 * @param filePath Path to json file.
	 */
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

	/**
	 * @brief Load scene from json file.
	 *
	 * Read components from a file, create entities and attach components to them.
	 *
	 * @param filePath Path to json file.
	 */
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

	/**
	 * @brief Creates custom Entity object plus entity in registry.
	 *
	 * Entity object is useful because it's more intuitive to call
	 * entityObject.GetComponent<ComponentType>() rather than m_Scene->m_Registry.get<ComponentType>(nativeEnttEntity).
	 *
	 * @param name Name of the entity which will be in EntityTagComponent.
	 */
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), name, this };
		return entity;
	}

	CameraComponent& Scene::GetCamera()
	{
		auto view = m_Registry.view<CameraComponent, EntityTagComponent>();
		for (auto entity : view)
		{
			auto& tag = view.get<EntityTagComponent>(entity);
			if (tag.Name == "Main Camera")
				return view.get<CameraComponent>(entity);
		}
	}

}