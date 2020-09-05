#include "pch.h"
#include "Yugo.h"

#include "Components.h"
#include "Script.h"
#include "GameObject.h"


class PlayerScript : public Script
{
private:
	std::vector<GameObject> m_Enemies;

public:
	PlayerScript() { m_ScriptFilePath = __FILE__; }

	void OnStart() override
	{
		m_Enemies = GameObject::FindGameObjectsWithTag("sphere");
	}

	void OnUpdate(float ts) override
	{
		if (IsKeyboardkeyPressed(KEY_W))
		{
			auto& transform = GetComponent<TransformComponent>();
			transform.Position.x -= 5.0f;
			
			for (auto enemy : m_Enemies)
			{
				auto& transform = enemy.GetComponent<TransformComponent>();
				transform.Position.x += 3.0f;
			}
		}
	}

	void OnShutdown() override
	{
		
	}
};


static ScriptRegister<PlayerScript> s_ScriptRegister;
