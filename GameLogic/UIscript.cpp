#include "pch.h"
#include "Yugo.h"

#include "Script.h"
#include "GameObject.h"
#include "Components.h"
#include "Events.h"


class UIscript : public Script
{
private:
	std::vector<GameObject> Buttons;

public:
	UIscript() { m_ScriptFilePath = __FILE__; }

	void OnStart() override
	{
		Buttons = GameObject::FindGameObjectsWithTag("button");
	}

	void OnUpdate(float ts) override
	{
		
	}

	void OnEvent(const Event& event) override
	{
		if (event.GetEventType() == EventType::MouseButtonPress)
		{
			const auto& mouseButtonPress = static_cast<const MouseButtonPress&>(event);
			if (mouseButtonPress.GetButtonCode() == MOUSE_BUTTON_LEFT)
			{
				auto& transform = GetComponent<TransformComponent>();
				if (IsMouseHoveringRect(transform))
				{
					std::cout << "Levi klik!\n";
				}
			}
		}
	}

	void OnShutdown() override
	{

	}
};


static ScriptRegister<UIscript> s_ScriptRegister;