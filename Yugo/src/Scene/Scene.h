#pragma once
#include "Renderer/MeshRenderer.h"
#include "Core/ResourceManager.h"
#include "MouseRay.h"
#include "Grid.h"

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

		using CloneFunctionType = void(const entt::registry&, entt::registry&);
		using TraverseFun = std::function<void(entt::entity)>;

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
		
		CameraComponent& GetCamera();
		
		template <typename ComponentType>
		static void CloneRegistry(const entt::registry& from, entt::registry& to)
		{
			const auto* data = from.data<ComponentType>(); // Array of entities that have specific component
			const auto size = from.size<ComponentType>(); // Size of array

			const auto* raw = from.raw<ComponentType>(); // Array of components
			if (raw != nullptr) // For some reason "visit" function iterates over pools that have components that are not added to registry ???
				to.insert<ComponentType>(data, data + size, raw, raw + size);
		}

		template<typename... ComponentTypes>
		auto GetView()
		{
			return m_Registry.view<ComponentTypes...>();
		}

		template<typename ComponentType>
		bool HasComponent(entt::entity entity)
		{
			return m_Registry.has<ComponentType>(entity);
		}

		template<typename ComponentType>
		auto& GetComponent(entt::entity entity)
		{
			return m_Registry.get<ComponentType>(entity);
		}

		template<typename ComponentType>
		std::vector<entt::entity>& FindEntitiesWithComponent()
		{
			static std::vector<entt::entity> entities;

			if (entities.size() > 0)
				entities.clear();

			auto view = m_Registry.view<ComponentType>();
			for (auto entity : view)
				entities.push_back(entity);
			return entities;
		}

		template<typename ComponentType>
		entt::entity FindEntityWithComponent()
		{
			auto view = m_Registry.view<ComponentType>();
			for (auto entity : view)
				return entity; // Returns first found entity.
			return entt::null;
		}

		template<typename ComponentType>
		std::vector<ComponentType*>& GetComponentsInChildren(entt::entity entity)
		{
			static std::vector<ComponentType*> components; // TODO: Check if it is OK to return pointer to entt component!

			// Clear already used vector with components
			if (components.size() > 0)
				components.clear();

			TraverseFun traverse = [&](entt::entity node) {
				auto& relationship = GetComponent<RelationshipComponent>(node);
				for (auto child : relationship.Children)
				{
					if (HasComponent<ComponentType>(child))
					{
						auto& canvas = GetComponent<ComponentType>(child);
						components.push_back(&canvas);
					}
				}

				for (auto child : relationship.Children)
					traverse(child);
			};

			traverse(entity);

			return components;
		}

	private:
		entt::registry m_Registry; // Registry is kind of in-memory database storage for entities and components.
		//uPtr<UserInterface> m_UserInterface;
		sPtr<Grid> m_Grid;
		std::unordered_map<entt::id_type, CloneFunctionType*> m_CloneFunctions;
	};

}