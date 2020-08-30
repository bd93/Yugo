#pragma once
#include "Behaviour.h"
#include "GameObject.h"

#define SCRIPT_API __declspec(dllexport)


/*
This class represents Base class for native scripts;
It has a pointer to Behaviour class in order to implement wrapper methods,
so script developer can use only method name instead of m_Behaviour->method_name
*/
class Script
{
public:
	virtual ~Script() {}

	virtual void OnStart() = 0;
	virtual void OnUpdate(float ts) = 0;
	virtual void OnShutdown() = 0;

	void SetBehaviour(Behaviour* scriptBehaviour) { m_Behaviour = scriptBehaviour; }
	Yugo::Entity* GetEntity() { return m_Behaviour->GetEntity(); }
	const std::string& GetScriptFilePath() { return m_ScriptFilePath; }
	
	bool IsKeyboardkeyPressed(Yugo::KeyCode key) { return m_Behaviour->IsKeyboardkeyPressed(key); }
	bool IsMouseButtonPressed(Yugo::MouseCode button) { return m_Behaviour->IsMouseButtonPressed(button); }

	template<typename T>
	T& GetComponent() {}

	template<>
	Yugo::TransformComponent& GetComponent<Yugo::TransformComponent>() { return m_Behaviour->GetTransformComponent(); }

	template<>
	Yugo::MeshComponent& GetComponent<Yugo::MeshComponent>() { return m_Behaviour->GetMeshComponent(); }

	template<>
	Yugo::AnimationComponent& GetComponent<Yugo::AnimationComponent>() { return m_Behaviour->GetAnimationComponent(); }

	template<typename T>
	bool HasComponent() {}

	template<>
	bool HasComponent<Yugo::TransformComponent>() { return m_Behaviour->HasTransformComponent(); }

	template<>
	bool HasComponent<Yugo::MeshComponent>() { return m_Behaviour->HasMeshComponent(); }

	template<>
	bool HasComponent<Yugo::AnimationComponent>() { return m_Behaviour->HasAnimationComponent(); }

	/*
	This static method initialize static std::vector inside it's body (this is needed because of explicit dll linking);
	A vector is used to register client scripts;
	Client scripts are being registered when scripts (.cpp files) are compiled;
	Inside these files client scripts use ScriptRegister template struct in order push that script to vector;
	*/
	static std::vector<Script*>& GetClientScripts();

protected:
	Behaviour* m_Behaviour;
	//IGameObject* m_GameObject; // TEMPORARY!!!
	uint32_t m_EntityId;
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