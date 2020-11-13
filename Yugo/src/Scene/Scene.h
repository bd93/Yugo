#pragma once
#include "Renderer/MeshRenderer.h"
#include "Core/ResourceManager.h"
#include "MouseRay.h"

#include <entt/entt.hpp>


namespace Yugo
{
	class Entity;
	class UserInterface;

	/**
	 * @brief Renderable scene
	 *
	 * Scene consists of entities, which are just unsigned integers under the hood.
	 * Each entity can have multiple components attached to it.
	 * Components contain only data, while logic is implemented in "system" classes.
	 */
	class Scene
	{
		friend class Entity;
		friend class Editor; // World editor
		friend class UserInterface; // Game UI

	public:
		Scene();

		void OnStart();
		void OnEvent(const Event& event);
		void OnUpdate(TimeStep ts);
		void OnRender();
		void OnShutdown();

		void SaveScene(const std::string& filePath);
		void LoadScene(const std::string& filePath);

		Entity CreateEntity(const std::string& name = "");

		template<typename... ComponentTypes>
		auto GetView()
		{
			return m_Registry.view<ComponentTypes...>();
		}

		template<typename ComponentType>
		auto& GetComponent(entt::entity entity)
		{
			return m_Registry.get<ComponentType>(entity);
		}

		template<typename ComponentType>
		bool HasComponent(entt::entity entity)
		{
			return m_Registry.has<ComponentType>(entity);
		}

	private:
		entt::registry m_Registry; // Registry is kind of in-memory database storage for entities and components
		uPtr<UserInterface> m_UserInterface;
	};

}