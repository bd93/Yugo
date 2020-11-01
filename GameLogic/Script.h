#pragma once
#include "ScriptInterface.h"
#include "GameObject.h"


namespace GameLogic
{

	/**
	 * @brief Base class for all client scripts.
	 *
	 * Methods are overriden on the game engine side in ScriptInterfaceImpl class.
	 * This class holds ScriptInterface pointer which points to ScriptInterfaceImpl.
	 * Methods call engine functionalities through the pointer.
	 */
	class Script
	{
	public:
		virtual ~Script() {}

		virtual void OnStart() = 0;
		virtual void OnUpdate(float ts) = 0;
		virtual void OnEvent(const Yugo::Event& event) = 0;
		virtual void OnShutdown() = 0;

		void SetScriptInterface(ScriptInterface* scriptInterface) { m_ScriptInterface = scriptInterface; }
		void SetGameObject(GameObject* gameObject) { m_GameObject = gameObject; }
		const std::string& GetScriptFilePath() { return m_ScriptFilePath; }

		bool IsKeyboardkeyPressed(Yugo::KeyCode key) { return m_ScriptInterface->IsKeyboardkeyPressed(key); }
		bool IsMouseButtonPressed(Yugo::MouseCode button) { return m_ScriptInterface->IsMouseButtonPressed(button); }
		bool IsMouseHoveringRect(TransformComponent& transform) { return m_ScriptInterface->IsMouseHoveringRect(transform); }
		//glm::vec2 GetMousePosition() { return m_ScriptInterface->GetMousePosition(); }

		template<typename T>
		T& GetComponent() {}

		template<>
		Yugo::TransformComponent& GetComponent<Yugo::TransformComponent>() { return m_ScriptInterface->GetTransformComponent(); }

		template<>
		Yugo::MeshComponent& GetComponent<Yugo::MeshComponent>() { return m_ScriptInterface->GetMeshComponent(); }

		template<>
		Yugo::AnimationComponent& GetComponent<Yugo::AnimationComponent>() { return m_ScriptInterface->GetAnimationComponent(); }

		template<>
		Yugo::EntityTagComponent& GetComponent<Yugo::EntityTagComponent>() { return m_ScriptInterface->GetEntityTagComponent(); }

		template<>
		Yugo::SpriteComponent& GetComponent<Yugo::SpriteComponent>() { return m_ScriptInterface->GetSpriteComponent(); }

		template<typename T>
		bool HasComponent() {}

		template<>
		bool HasComponent<Yugo::TransformComponent>() { return m_ScriptInterface->HasTransformComponent(); }

		template<>
		bool HasComponent<Yugo::MeshComponent>() { return m_ScriptInterface->HasMeshComponent(); }

		template<>
		bool HasComponent<Yugo::AnimationComponent>() { return m_ScriptInterface->HasAnimationComponent(); }

		template<>
		bool HasComponent<Yugo::EntityTagComponent>() { return m_ScriptInterface->HasEntityTagComponent(); }

		template<>
		bool HasComponent<Yugo::SpriteComponent>() { return m_ScriptInterface->HasSpriteComponent(); }

		/**
		 * @brief Returns client scripts
		 *
		 * This method initializes static vector that holds Script pointers.
		 * Client script is pushed into a vector when .cpp file is compiled,
		 * because each client script creates ScriptRegister static object.
		 * This object registers client script through template deduction.
		 *
		 * @note Static vector is created inside method's body because of weird error regarding explicit dll linking.
		 */
		static std::vector<Script*>& GetClientScripts();

	protected:
		ScriptInterface* m_ScriptInterface;
		GameObject* m_GameObject; // Game object which this script is attached to
		std::string m_ScriptFilePath;
	};

	/**
	 * @brief Client script registration
	 *
	 * Client script is registered when .cpp file is compiled,
	 * ScriptRegister object registers client script through template deduction.
	 */
	template<typename ClientScript>
	struct ScriptRegister
	{
		ScriptRegister()
		{
			ClientScript* clientScript = new ClientScript();
			Script::GetClientScripts().push_back(clientScript);
		}
	};

	/**
	 * @brief Array of client script pointers
	 *
	 * This struct holds array of Script pointers and it's size.
	 * Array of script pointers is needed in order to transform std::vector to classic dynamic array,
	 * so it can be exported through extern "C" dll function
	 * 
	 * @note Standard vector can't be exported via extern "C" dll function
	 */
	struct ScriptArray
	{
		size_t Size;
		Script** Scripts;
	};

}

