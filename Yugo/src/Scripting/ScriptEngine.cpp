#include "pch.h"

#include "ScriptEngine.h"
#include "Input/UserInput.h"
//#include "EngineFuncs.h"


namespace Yugo
{

	ScriptEngine::ScriptEngine()
		: m_Scene(nullptr)
	{
	}

	/**
	 * @brief Method to be called during application OnStart stage.
	 *
	 * This method calls dll exported functions and creates client scripts and game objects.
	 * Then it creates interface implementations and sets it in client scripts and game objects.
	 * It also pushes interfaces and game objects to vectors in order to destroy them when OnStop method is called. 
	 */
	void Yugo::ScriptEngine::OnStart(Scene* scene, Window* window)
	{
#ifdef YU_RELEASE
		Dispatcher::Subscribe<KeyboardKeyPress>(this);
		Dispatcher::Subscribe<MouseButtonPress>(this);
		Dispatcher::Subscribe<MouseScroll>(this);
		Dispatcher::Subscribe<ImportAssetEvent>(this);
#endif

		m_Scene = scene;
		m_Window = window;

		m_Lib = LoadLibrary(L"GameLogic.dll");
		if (m_Lib == NULL)
			std::cout << "Cannot dynamically load GameLogic.dll!\n";

		CreateScripts NewScripts = (CreateScripts)GetProcAddress(m_Lib, "CreateScripts");
		if (NewScripts == NULL)
			std::cout << "Cannot load CreateScripts function!\n";
		CreateGameObject NewGameObject = (CreateGameObject)GetProcAddress(m_Lib, "CreateGameObject");
		if (NewGameObject == NULL)
			std::cout << "Cannot load CreateGameObject function!\n";
		GetScriptFilePath ScriptFilePath = (GetScriptFilePath)GetProcAddress(m_Lib, "GetScriptFilePath");
		if (ScriptFilePath == NULL)
			std::cout << "Cannot load GetScriptFilePath function!\n";
		SetGameObject SetScriptGameObject = (SetGameObject)GetProcAddress(m_Lib, "SetGameObject");
		if (SetScriptGameObject == NULL)
			std::cout << "Cannot load SetGameObject function!\n";
		ImportGameEngineFuncs SetGameEngineFuncs = (ImportGameEngineFuncs)GetProcAddress(m_Lib, "ImportGameEngineFuncs");
		if (SetGameEngineFuncs == NULL)
			std::cout << "Cannot load ImportGameEngineFuncs function!\n";

		GameLogic::GameEngineFuncs gameEngineFuncs;

		BindGameEngineFunctionalities(gameEngineFuncs);

		SetGameEngineFuncs(gameEngineFuncs);

		m_ScriptArray = NewScripts();

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
		{
			//Entity& entity = m_ScriptEntityMap[ScriptFilePath(m_ScriptArray.Scripts[i])];
			auto entity = m_ScriptEntityMap[ScriptFilePath(m_ScriptArray.Scripts[i])];
			auto& scriptComponent = m_Scene->GetComponent<ScriptComponent>(entity);
			scriptComponent.ClientScript = m_ScriptArray.Scripts[i];

			GameLogic::GameObject* gameObject = NewGameObject(entity);
			m_GameObjects.push_back(gameObject);
			SetScriptGameObject(gameObject, m_ScriptArray.Scripts[i]);
		}

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnStart();
	}

	/* @brief Update scripts each frame. */
	void Yugo::ScriptEngine::OnUpdate(TimeStep ts)
	{
		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
			m_ScriptArray.Scripts[i]->OnUpdate(ts);
	}

	/* @brief Calls OnEvent each frame in each client's script. */
	void ScriptEngine::OnEvent(const Event& event)
	{
		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
		{
			auto eventType = event.GetEventType();
			
			switch (eventType)
			{
			case EventType::KeyboardKeyPress:
				break;
			case EventType::KeyboardKeyRelease:
				break;
			case EventType::MouseButtonPress:
			{
				const auto& mouseButtonPress = static_cast<const MouseButtonPress&>(event);
				if (mouseButtonPress.GetButtonCode() == MOUSE_BUTTON_LEFT)
					m_ScriptArray.Scripts[i]->OnMouseLeftClick();
				if (mouseButtonPress.GetButtonCode() == MOUSE_BUTTON_RIGHT)
					m_ScriptArray.Scripts[i]->OnMouseRightClick();
				break;
			}
			case EventType::MouseButtonRelease:
			{
				const auto& mouseButtonRelease = static_cast<const MouseButtonRelease&>(event);
				if (mouseButtonRelease.GetButtonCode() == MOUSE_BUTTON_LEFT)
					m_ScriptArray.Scripts[i]->OnMouseLeftRelease();
				if (mouseButtonRelease.GetButtonCode() == MOUSE_BUTTON_RIGHT)
					m_ScriptArray.Scripts[i]->OnMouseRightRelease();
				break;
			}
			case EventType::MouseScroll:
				break;
			case EventType::MouseMove:
				break;
			case EventType::WindowClose:
				break;
			case EventType::WindowResize:
				break;
			case EventType::WindowFocus:
				break;
			case EventType::WindowLostFocus:
				break;
			case EventType::WindowMove:
				break;
			case EventType::ImportAsset:
				break;
			default:
				break;
			}
		}
	}

	/**
	 * @brief Method to be called when script is stopped.
	 *
	 * OnStop calls exported dll functions in order to destroy objects allocated on dll heap.
	 * It also clears vectors so they can be used for the next script running state. 
	 */
	void ScriptEngine::OnStop()
	{
		DeleteGameObject DestroyGameObject = (DeleteGameObject)GetProcAddress(m_Lib, "DeleteGameObject");
		if (DestroyGameObject == NULL)
			std::cout << "Cannot load DeleteGameObject function!\n";

		for (uint32_t i = 0; i < m_ScriptArray.Size; ++i)
		{
			DestroyGameObject(m_GameObjects[i]);
		}

		m_GameObjects.clear();

		DeleteScripts DestroyScripts = (DeleteScripts)GetProcAddress(m_Lib, "DeleteScripts");
		if (DestroyScripts == NULL)
			std::cout << "Cannot load DeleteScripts function!\n";
		DestroyScripts(m_ScriptArray);
				
		FreeLibrary(m_Lib);
	}

	void ScriptEngine::OnReload()
	{
		// TODO: Consider if this method is necessary or not.
	}

	/**
	 * @brief Keeps track of script file path and which entity this script is attached to.
	 *
	 * This method is called when user clicks "Attach Script" in world editor.
	 * The main idea is to fetch entity in ScriptEngine::OnStart method and sets it in script interface and game object.
	 *
	 * @param scriptFilePath Path to script file.
	 * @param entity Custom entity which holds entt::entity, name and pointer to scene.
	 */
	void ScriptEngine::AttachScript(const std::string& scriptFilePath, entt::entity entity)
	{
		if (m_ScriptEntityMap.find(scriptFilePath) == m_ScriptEntityMap.end())
			m_ScriptEntityMap[scriptFilePath] = entity;
	}

	/* @brief Scene setter */
	void ScriptEngine::SetScene(Scene* scene)
	{
		m_Scene = scene;
	}

	void ScriptEngine::BindGameEngineFunctionalities(GameLogic::GameEngineFuncs& gameEngineFuncs)
	{
		gameEngineFuncs.IsKeyboardKeyPressed = UserInput::IsKeyboardKeyPressed;
		gameEngineFuncs.IsMouseButtonPressed = UserInput::IsMouseButtonPressed;

		gameEngineFuncs.GetWindowWidth = [this]() {return m_Window->GetWindowWidth(); };
		gameEngineFuncs.GetWindowHeight = [this]() {return m_Window->GetWindowHeight(); };

		gameEngineFuncs.GetMousePosX = UserInput::GetMousePosX;
		gameEngineFuncs.GetMousePosY = UserInput::GetMousePosY;

		gameEngineFuncs.GetMousePosition = UserInput::GetMousePosition;
		gameEngineFuncs.GetWindowSize = [this]() {return m_Window->GetWindowSize(); };

		gameEngineFuncs.CalculateRayOrigin = MouseRay::CalculateRayOrigin;
		gameEngineFuncs.CheckCollisionWithMesh = MouseRay::CheckCollisionWithMesh;
		gameEngineFuncs.CheckCollisionWithPlane = MouseRay::CheckCollisionWithPlane;

		//gameEngineFuncs.HasTransformComponent = std::bind(&Scene::template HasComponent<TransformComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.HasMeshComponent = std::bind(&Scene::template HasComponent<MeshComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.HasSpriteComponent = std::bind(&Scene::template HasComponent<SpriteComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.HasAnimationComponent = std::bind(&Scene::template HasComponent<AnimationComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.HasEntityTagComponent = std::bind(&Scene::template HasComponent<EntityTagComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.HasCameraComponent = std::bind(&Scene::template HasComponent<CameraComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.HasScriptComponent = std::bind(&Scene::template HasComponent<ScriptComponent>, m_Scene, std::placeholders::_1);

		//gameEngineFuncs.GetTransformComponent = std::bind(&Scene::template GetComponent<TransformComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.GetMeshComponent = std::bind(&Scene::template GetComponent<MeshComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.GetSpriteComponent = std::bind(&Scene::template GetComponent<SpriteComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.GetAnimationComponent = std::bind(&Scene::template GetComponent<AnimationComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.GetEntityTagComponent = std::bind(&Scene::template GetComponent<EntityTagComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.GetCameraComponent = std::bind(&Scene::template GetComponent<CameraComponent>, m_Scene, std::placeholders::_1);
		//gameEngineFuncs.GetScriptComponent = std::bind(&Scene::template GetComponent<ScriptComponent>, m_Scene, std::placeholders::_1);

		gameEngineFuncs.HasTransformComponent = [this](entt::entity entity) { return m_Scene->HasComponent<TransformComponent>(entity); };
		gameEngineFuncs.HasMeshComponent = [this](entt::entity entity) { return m_Scene->HasComponent<MeshComponent>(entity); };
		gameEngineFuncs.HasSpriteComponent = [this](entt::entity entity) { return m_Scene->HasComponent<SpriteComponent>(entity); };
		gameEngineFuncs.HasAnimationComponent = [this](entt::entity entity) { return m_Scene->HasComponent<AnimationComponent>(entity); };
		gameEngineFuncs.HasEntityTagComponent = [this](entt::entity entity) { return m_Scene->HasComponent<EntityTagComponent>(entity); };
		gameEngineFuncs.HasCameraComponent = [this](entt::entity entity) { return m_Scene->HasComponent<CameraComponent>(entity); };
		gameEngineFuncs.HasScriptComponent = [this](entt::entity entity) { return m_Scene->HasComponent<ScriptComponent>(entity); };

		gameEngineFuncs.GetTransformComponent = [this](entt::entity entity) -> auto& { return m_Scene->GetComponent<TransformComponent>(entity); };
		gameEngineFuncs.GetMeshComponent = [this](entt::entity entity) -> auto& { return m_Scene->GetComponent<MeshComponent>(entity); };
		gameEngineFuncs.GetSpriteComponent = [this](entt::entity entity) -> auto& { return m_Scene->GetComponent<SpriteComponent>(entity); };
		gameEngineFuncs.GetAnimationComponent = [this](entt::entity entity) -> auto& { return m_Scene->GetComponent<AnimationComponent>(entity); };
		gameEngineFuncs.GetEntityTagComponent = [this](entt::entity entity) -> auto& { return m_Scene->GetComponent<EntityTagComponent>(entity); };
		gameEngineFuncs.GetCameraComponent = [this](entt::entity entity) -> auto& { return m_Scene->GetComponent<CameraComponent>(entity); };
		gameEngineFuncs.GetScriptComponent = [this](entt::entity entity) -> auto& { return m_Scene->GetComponent<ScriptComponent>(entity); };

		gameEngineFuncs.FindEntitiesWithTransformComponent = [this]() -> auto& {return m_Scene->FindEntitiesWithComponent<TransformComponent>(); };
		gameEngineFuncs.FindEntitiesWithMeshComponent = [this]() -> auto& {return m_Scene->FindEntitiesWithComponent<MeshComponent>(); };
		gameEngineFuncs.FindEntitiesWithSpriteComponent = [this]() -> auto& {return m_Scene->FindEntitiesWithComponent<SpriteComponent>(); };
		gameEngineFuncs.FindEntitiesWithAnimationComponent = [this]() -> auto& {return m_Scene->FindEntitiesWithComponent<AnimationComponent>(); };
		gameEngineFuncs.FindEntitiesWithEntityTagComponent = [this]() -> auto& {return m_Scene->FindEntitiesWithComponent<EntityTagComponent>(); };
		gameEngineFuncs.FindEntitiesWithCameraComponent = [this]() -> auto& {return m_Scene->FindEntitiesWithComponent<CameraComponent>(); };
		gameEngineFuncs.FindEntitiesWithScriptComponent = [this]() -> auto& {return m_Scene->FindEntitiesWithComponent<ScriptComponent>(); };

		gameEngineFuncs.FindEntityWithTransformComponent = [this]() {return m_Scene->FindEntityWithComponent<TransformComponent>(); };
		gameEngineFuncs.FindEntityWithMeshComponent = [this]() {return m_Scene->FindEntityWithComponent<MeshComponent>(); };
		gameEngineFuncs.FindEntityWithSpriteComponent = [this]() {return m_Scene->FindEntityWithComponent<SpriteComponent>(); };
		gameEngineFuncs.FindEntityWithAnimationComponent = [this]() {return m_Scene->FindEntityWithComponent<AnimationComponent>(); };
		gameEngineFuncs.FindEntityWithEntityTagComponent = [this]() {return m_Scene->FindEntityWithComponent<EntityTagComponent>(); };
		gameEngineFuncs.FindEntityWithCameraComponent = [this]() {return m_Scene->FindEntityWithComponent<CameraComponent>(); };
		gameEngineFuncs.FindEntityWithScriptComponent = [this]() {return m_Scene->FindEntityWithComponent<ScriptComponent>(); };

		gameEngineFuncs.GetTransformComponentsInChildren = [this](entt::entity entity) ->auto& {return m_Scene->GetComponentsInChildren<TransformComponent>(entity); };
		gameEngineFuncs.GetMeshComponentsInChildren = [this](entt::entity entity) ->auto& {return m_Scene->GetComponentsInChildren<MeshComponent>(entity); };
		gameEngineFuncs.GetSpriteComponentsInChildren = [this](entt::entity entity) ->auto& {return m_Scene->GetComponentsInChildren<SpriteComponent>(entity); };
		gameEngineFuncs.GetAnimationComponentsInChildren = [this](entt::entity entity) ->auto& {return m_Scene->GetComponentsInChildren<AnimationComponent>(entity); };
		gameEngineFuncs.GetEntityTagComponentsInChildren= [this](entt::entity entity) ->auto& {return m_Scene->GetComponentsInChildren<EntityTagComponent>(entity); };
		gameEngineFuncs.GetCameraComponentsInChildren = [this](entt::entity entity) ->auto& {return m_Scene->GetComponentsInChildren<CameraComponent>(entity); };
		gameEngineFuncs.GetScriptComponentsInChildren = [this](entt::entity entity) ->auto& {return m_Scene->GetComponentsInChildren<ScriptComponent>(entity); };
	}

}
