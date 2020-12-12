#include "pch.h"
#include "ActionBarUI.h"
#include "GameScripts/PlayerScript.h"


namespace GameLogic
{

	void ActionBarUI::OnStart()
	{
		Buttons = GameObject::FindGameObjectsWithComponent<ButtonWidgetComponent>();
	}

	void ActionBarUI::OnUpdate(TimeStep ts)
	{
	}

	void ActionBarUI::OnEvent(const Event& event)
	{
		if (event.GetEventType() == EventType::MouseButtonPress)
		{
			const auto& mouseButtonPress = static_cast<const MouseButtonPress&>(event);
			if (mouseButtonPress.GetButtonCode() == MOUSE_BUTTON_LEFT)
			{
				for (auto& button : Buttons)
				{
					auto& transform = button.GetComponent<TransformComponent>();
					if (Input::IsMouseHoveringRect(transform))
					{
						auto& player = GameObject::FindGameObjectWithTag("cube");
						auto& script = player.GetComponent<ScriptComponent>();
						auto playerScript = static_cast<PlayerScript*>(script.ClientScript);
						//playerScript->Move();
						std::cout << "radi\n";
					}
				}
			}
		}
	}

	void ActionBarUI::OnShutdown()
	{
	}

	//REGISTER_CLASS(ActionBarUI, __FILE__)

}
