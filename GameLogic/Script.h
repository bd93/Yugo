#pragma once
#include "ScriptInterface.h"
#include "GameObject.h"


/*
This class represents Base class for native scripts;
It has a pointer to Behaviour class in order to implement wrapper methods,
so script developer can use only method name instead of m_ScriptInterface->method_name
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

	/*
	This static method initialize static std::vector inside it's body (this is needed because of explicit dll linking);
	A vector is used to register client scripts;
	Client scripts are being registered when scripts (.cpp files) are compiled;
	Inside these files client scripts use ScriptRegister template struct in order push that script to vector;
	*/
	static std::vector<Script*>& GetClientScripts();

protected:
	ScriptInterface* m_ScriptInterface;
	GameObject* m_GameObject; // Game object which this script is attached to
	std::string m_ScriptFilePath;
};


template<typename T>
struct ScriptRegister
{
	ScriptRegister() 
	{ 
		T* clientScript = new T();
		Script::GetClientScripts().push_back(clientScript);
	}
};

/*
This struct holds array of Script pointers and it's size;
Array of script pointers is needed in order to transform std::vector to POD,
so it can be exported in extern "C" dll exported function
*/
struct ScriptArray
{
	size_t Size;
	Script** Scripts;
};