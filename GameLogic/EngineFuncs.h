#pragma once
#include "Yugo.h"

#include <functional>


namespace GameLogic
{

	struct GameEngineFuncs
	{

		std::function<bool(KeyCode)> IsKeyboardKeyPressed;
		std::function<bool(MouseCode)> IsMouseButtonPressed;

		std::function<int(void)> GetWindowWidth;
		std::function<int(void)> GetWindowHeight;

		std::function<float(void)> GetMousePosX;
		std::function<float(void)> GetMousePosY;

		std::function<std::pair<float, float>(void)> GetMousePosition;
		std::function<std::pair<int, int>(void)> GetWindowSize;

		std::function<void(const CameraComponent&, float, float, int, int, Yugo::MouseRay::RayInfo*)> CalculateRayOrigin;
		std::function<bool(const MeshComponent&, const TransformComponent&, Yugo::MouseRay::RayInfo*)> CheckCollisionWithMesh;
		std::function<bool(const glm::vec3&, float, Yugo::MouseRay::RayInfo*)> CheckCollisionWithPlane;

		std::function<bool(entt::entity)> HasTransformComponent;
		std::function<bool(entt::entity)> HasMeshComponent;
		std::function<bool(entt::entity)> HasSpriteComponent;
		std::function<bool(entt::entity)> HasAnimationComponent;
		std::function<bool(entt::entity)> HasEntityTagComponent;
		std::function<bool(entt::entity)> HasCameraComponent;
		std::function<bool(entt::entity)> HasScriptComponent;

		std::function<TransformComponent& (entt::entity)> GetTransformComponent;
		std::function<MeshComponent& (entt::entity)> GetMeshComponent;
		std::function<SpriteComponent& (entt::entity)> GetSpriteComponent;
		std::function<AnimationComponent& (entt::entity)> GetAnimationComponent;
		std::function<EntityTagComponent& (entt::entity)> GetEntityTagComponent;
		std::function<CameraComponent& (entt::entity)> GetCameraComponent;
		std::function<ScriptComponent& (entt::entity)> GetScriptComponent;

		std::function<std::vector<entt::entity>&(void)> FindEntitiesWithTransformComponent;
		std::function<std::vector<entt::entity>&(void)> FindEntitiesWithMeshComponent;
		std::function<std::vector<entt::entity>&(void)> FindEntitiesWithSpriteComponent;
		std::function<std::vector<entt::entity>&(void)> FindEntitiesWithAnimationComponent;
		std::function<std::vector<entt::entity>&(void)> FindEntitiesWithEntityTagComponent;
		std::function<std::vector<entt::entity>&(void)> FindEntitiesWithCameraComponent;
		std::function<std::vector<entt::entity>&(void)> FindEntitiesWithScriptComponent;

		std::function<entt::entity(void)> FindEntityWithTransformComponent;
		std::function<entt::entity(void)> FindEntityWithMeshComponent;
		std::function<entt::entity(void)> FindEntityWithSpriteComponent;
		std::function<entt::entity(void)> FindEntityWithAnimationComponent;
		std::function<entt::entity(void)> FindEntityWithEntityTagComponent;
		std::function<entt::entity(void)> FindEntityWithCameraComponent;
		std::function<entt::entity(void)> FindEntityWithScriptComponent;

		std::function<std::vector<TransformComponent*>& (entt::entity)> GetTransformComponentsInChildren;
		std::function<std::vector<MeshComponent*>& (entt::entity)> GetMeshComponentsInChildren;
		std::function<std::vector<SpriteComponent*>& (entt::entity)> GetSpriteComponentsInChildren;
		std::function<std::vector<AnimationComponent*>& (entt::entity)> GetAnimationComponentsInChildren;
		std::function<std::vector<EntityTagComponent*>& (entt::entity)> GetEntityTagComponentsInChildren;
		std::function<std::vector<CameraComponent*>& (entt::entity)> GetCameraComponentsInChildren;
		std::function<std::vector<ScriptComponent*>& (entt::entity)> GetScriptComponentsInChildren;
	};


	struct UiEngineFuncs
	{
		//std::function<CanvasComponent& (entt::entity)> GetCanvasComponent;
		//std::function<ButtonComponent& (entt::entity)> GetButtonComponent;
		//std::function<TextComponent& (entt::entity)> GetTextComponent;
	};


	class EngineFuncs
	{
	public:
		static void SetGameEngineFuncs(GameEngineFuncs gameEngineFuncs)
		{
			s_GameEngineFuncs = gameEngineFuncs;
		}

		static GameEngineFuncs& GetGameEngineFuncs()
		{
			//static GameEngineFuncs gameEngineFuncs;
			//return gameEngineFuncs;
			return s_GameEngineFuncs;
		}
		
		static UiEngineFuncs& GetUiEngineFuncs()
		{
			static UiEngineFuncs uiEngineFuncs;
			return uiEngineFuncs;
		}

	private:
		static GameEngineFuncs s_GameEngineFuncs;

	};

}