#pragma once
#include "Renderer/MeshRenderer.h"
#include "Core/ResourceManager.h"
#include "MouseRay.h"

#include <entt/entt.hpp>


namespace Yugo
{
	class Entity;

	/*
	This class represents renderable scene;
	Entities, which are just unsigned integer IDs, can have attached components,
	which define behavior for specific entity. 
	Components contain only data, while the logic is implemented in systems;
	*/
	class Scene
	{
		friend class Entity;
		friend class Editor;
	
	public:
		Scene();

		void OnStart();
		void OnEvent(const Event& event);
		void OnUpdate(float ts);
		void OnRender();
		void OnShutdown();

		void SaveScene(const std::string& filePath);
		void LoadScene(const std::string& filePath);

		Entity CreateEntity(const std::string& name = "");

		void CastMouseRay(float mousePosX, float mousePosY, int sceneWidth, int sceneHeight);
		bool IsAnyMeshSelected();
		TransformComponent& GetSelectedMeshTransform();

		// TEMPORARY!!!
		entt::registry& GetRegistry();

	private:
		entt::registry m_Registry; // Registry is kind of in-memory database storage for entities and components
		entt::entity m_SelectedEntity; // Entity which is selected with mouse ray, on left click
		sPtr<Camera> m_Camera;
		MouseRay m_MouseRay;
	};

}