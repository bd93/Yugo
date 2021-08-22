#include "pch.h"
#include "Yugo.h"
#include "MainMenu.h"


namespace GameLogic
{

	void MainMenu::OnStart()
	{
		m_Player = GameObject::FindGameObjectWithTagName("Main Player");

		Widget* canvas = CreateWidget(Widget::Type::Canvas, GetRoot());
		canvas->m_Position = glm::vec2(100.0f, 100.0f);
		canvas->m_Size = glm::vec2(50.0f, 50.0f);
		canvas->m_BackgroundColor = glm::vec4(1.0f, 0.5f, 0.3f, 0.5f);
		canvas->OnMouseLeftClick([this]() {std::cout << "clicked!\n"; });
	}

	void MainMenu::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyboardKeyPressed(KEY_W))
		{
			
		}

	}

	void MainMenu::OnMouseLeftClick()
	{
		//auto [posX, posY] = Input::GetMousePosition();
	}



	REGISTER_CLASS(MainMenu, __FILE__)

}