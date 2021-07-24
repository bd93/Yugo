#pragma once
#include "YugoComponents.h"
//#include "Scene/Entity.h"
#include "EngineFuncs.h"
#include "Common.h"


namespace GameLogic
{

	/* @brief Scriptable game object in the scene. */
	class GameObject
	{
	public:
		void SetEntity(entt::entity entity);

		template<typename ComponentType>
		bool HasComponent() {}
		template<>
		bool HasComponent<TransformComponent>() { return EngineFuncs::GetGameEngineFuncs().HasTransformComponent(m_Entity); }
		template<>
		bool HasComponent<MeshComponent>() { return EngineFuncs::GetGameEngineFuncs().HasMeshComponent(m_Entity); }
		template<>
		bool HasComponent<SpriteComponent>() { return EngineFuncs::GetGameEngineFuncs().HasSpriteComponent(m_Entity); }
		template<>
		bool HasComponent<AnimationComponent>() { return EngineFuncs::GetGameEngineFuncs().HasAnimationComponent(m_Entity); }
		template<>
		bool HasComponent<EntityTagComponent>() { return EngineFuncs::GetGameEngineFuncs().HasEntityTagComponent(m_Entity); }
		template<>
		bool HasComponent<CameraComponent>() { return EngineFuncs::GetGameEngineFuncs().HasCameraComponent(m_Entity); }
		template<>
		bool HasComponent<ScriptComponent>() { return EngineFuncs::GetGameEngineFuncs().HasScriptComponent(m_Entity); }

		template<typename ComponentType>
		ComponentType& GetComponent() {}
		template<>
		TransformComponent& GetComponent<TransformComponent>() { return EngineFuncs::GetGameEngineFuncs().GetTransformComponent(m_Entity); }
		template<>
		MeshComponent& GetComponent<MeshComponent>() { return EngineFuncs::GetGameEngineFuncs().GetMeshComponent(m_Entity); }
		template<>
		SpriteComponent& GetComponent<SpriteComponent>() { return EngineFuncs::GetGameEngineFuncs().GetSpriteComponent(m_Entity); }
		template<>
		AnimationComponent& GetComponent<AnimationComponent>() { return EngineFuncs::GetGameEngineFuncs().GetAnimationComponent(m_Entity); }
		template<>
		EntityTagComponent& GetComponent<EntityTagComponent>() { return EngineFuncs::GetGameEngineFuncs().GetEntityTagComponent(m_Entity); }
		template<>
		CameraComponent& GetComponent<CameraComponent>() { return EngineFuncs::GetGameEngineFuncs().GetCameraComponent(m_Entity); }
		template<>
		ScriptComponent& GetComponent<ScriptComponent>() { return EngineFuncs::GetGameEngineFuncs().GetScriptComponent(m_Entity); }

		template<typename ComponentType>
		std::vector<ComponentType*>& GetComponentsInChildren() {}
		template<>
		std::vector<TransformComponent*>& GetComponentsInChildren() { return EngineFuncs::GetGameEngineFuncs().GetTransformComponentsInChildren(m_Entity); }
		template<>
		std::vector<MeshComponent*>& GetComponentsInChildren() { return EngineFuncs::GetGameEngineFuncs().GetMeshComponentsInChildren(m_Entity); }
		template<>
		std::vector<SpriteComponent*>& GetComponentsInChildren() { return EngineFuncs::GetGameEngineFuncs().GetSpriteComponentsInChildren(m_Entity); }
		template<>
		std::vector<AnimationComponent*>& GetComponentsInChildren() { return EngineFuncs::GetGameEngineFuncs().GetAnimationComponentsInChildren(m_Entity); }
		template<>
		std::vector<EntityTagComponent*>& GetComponentsInChildren() { return EngineFuncs::GetGameEngineFuncs().GetEntityTagComponentsInChildren(m_Entity); }
		template<>
		std::vector<CameraComponent*>& GetComponentsInChildren() { return EngineFuncs::GetGameEngineFuncs().GetCameraComponentsInChildren(m_Entity); }
		template<>
		std::vector<ScriptComponent*>& GetComponentsInChildren() { return EngineFuncs::GetGameEngineFuncs().GetScriptComponentsInChildren(m_Entity); }

		template<typename ComponentType>
		static std::vector<GameObject> FindGameObjectsWithComponent() {}
		template<>
		static std::vector<GameObject> FindGameObjectsWithComponent<TransformComponent>() { return GetGameObjectsWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntitiesWithTransformComponent); }
		template<>
		static std::vector<GameObject> FindGameObjectsWithComponent<MeshComponent>() { return GetGameObjectsWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntitiesWithMeshComponent); }
		template<>
		static std::vector<GameObject> FindGameObjectsWithComponent<SpriteComponent>() { return GetGameObjectsWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntitiesWithSpriteComponent); }
		template<>
		static std::vector<GameObject> FindGameObjectsWithComponent<AnimationComponent>() { return GetGameObjectsWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntitiesWithAnimationComponent); }
		template<>
		static std::vector<GameObject> FindGameObjectsWithComponent<EntityTagComponent>() { return GetGameObjectsWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntitiesWithEntityTagComponent); }
		template<>
		static std::vector<GameObject> FindGameObjectsWithComponent<CameraComponent>() { return GetGameObjectsWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntitiesWithCameraComponent); }
		template<>
		static std::vector<GameObject> FindGameObjectsWithComponent<ScriptComponent>() { return GetGameObjectsWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntitiesWithScriptComponent); }

		template<typename ComponentType>
		static GameObject FindGameObjectWithComponent() {}
		template<>
		static GameObject FindGameObjectWithComponent<TransformComponent>() { return GetGameObjectWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntityWithTransformComponent); }
		template<>
		static GameObject FindGameObjectWithComponent<MeshComponent>() { return GetGameObjectWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntityWithMeshComponent); }
		template<>
		static GameObject FindGameObjectWithComponent<SpriteComponent>() { return GetGameObjectWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntityWithSpriteComponent); }
		template<>
		static GameObject FindGameObjectWithComponent<AnimationComponent>() { return GetGameObjectWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntityWithAnimationComponent); }
		template<>
		static GameObject FindGameObjectWithComponent<EntityTagComponent>() { return GetGameObjectWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntityWithEntityTagComponent); }
		template<>
		static GameObject FindGameObjectWithComponent<CameraComponent>() { return GetGameObjectWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntityWithCameraComponent); }
		template<>
		static GameObject FindGameObjectWithComponent<ScriptComponent>() { return GetGameObjectWithComponent(EngineFuncs::GetGameEngineFuncs().FindEntityWithScriptComponent); }

		static GameObject FindGameObjectWithTagName(const std::string& tagName);
	
	private:
		std::string m_Name;
		entt::entity m_Entity;

		static std::vector<GameObject> GetGameObjectsWithComponent(std::function<std::vector<entt::entity>(void)> findEntities);
		static GameObject GetGameObjectWithComponent(std::function<entt::entity(void)> findEntities);
	};

}