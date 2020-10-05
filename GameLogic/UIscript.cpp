#include "pch.h"
#include "Yugo.h"

#include "Script.h"
#include "GameObject.h"
#include "Components.h"
#include "Events.h"


class UIscript : public Script
{
private:
	//std::vector<sPtr<ButtonWidgetComponent>> m_Buttons;
	std::vector<ButtonWidgetComponent*> m_Buttons;

public:
	UIscript() { m_ScriptFilePath = __FILE__; }

	void OnStart() override
	{
		m_Buttons = m_GameObject->GetComponentsInChildren<ButtonWidgetComponent>();
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
				auto& transform = m_GameObject->GetComponentsInChildren<TransformComponent>();
				int index = 0;
				for (auto& button : m_Buttons)
				{
					if (IsMouseHoveringRect(*transform[index]))
					{
						transform[index]->Scale.y += 5.0f;
					}
					index++;
				}
			}
		}
	}

	void OnShutdown() override
	{

	}
};


static ScriptRegister<UIscript> s_ScriptRegister;