#include "pch.h"
#include "Script.h"
#include "Yugo.h"


class PlayerScript : public Script
{
private:
	glm::vec3 m_Velocity = {5.0f, 5.0f, 2.0f};

public:
	PlayerScript() { m_ScriptFilePath = __FILE__; }

	void OnStart() override
	{
		
	}

	void OnUpdate(float ts) override
	{
		if (IsKeyboardkeyPressed(KEY_W))
		{
			auto& transform = GetComponent<Yugo::TransformComponent>();
			transform.Position.x -= 1.0f;
		}
	}

	void OnShutdown() override
	{
		
	}
};


static ScriptRegister<PlayerScript> s_ScriptRegister;
