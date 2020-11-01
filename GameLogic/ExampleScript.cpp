#include "pch.h"
#include "Yugo.h"

#include "Script.h"
#include "GameObject.h"
#include "Components.h"
#include "Events.h"



namespace GameLogic
{

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
					transform.Position.x += 1.0f;
				}
			}
		}

		void OnEvent(const Event& event) override
		{
			if (event.GetEventType() == EventType::MouseButtonPress)
			{
				const auto& mouseButtonPress = static_cast<const MouseButtonPress&>(event);
				if (mouseButtonPress.GetButtonCode() == MOUSE_BUTTON_LEFT)
				{

				}
			}
		}

		void OnShutdown() override
		{

		}
	};


	static ScriptRegister<PlayerScript> s_ScriptRegister;

}

