#include "pch.h"
#include "Yugo.h"
#include "MainMenu.h"
#include "GameScripts/PlayerScript.h"


namespace GameLogic
{

	void MainMenu::OnStart()
	{
		m_Player = GameObject::FindGameObjectWithTagName("Main Player");
		m_PlayerScript = m_Player.GetComponent<ScriptComponent>().ClientScript;

		//Widget* canvas = CreateWidget(Widget::Type::Canvas, GetRoot());
		//canvas->m_Position = glm::vec2(100.0f, 100.0f);
		//canvas->m_Size = glm::vec2(50.0f, 50.0f);
		//canvas->m_BackgroundColor = glm::vec4(1.0f, 0.5f, 0.3f, 0.5f);
		//canvas->OnMouseLeftClick([this]() {std::cout << "clicked!\n"; });

		Widget* widget = CreateWidget(Widget::Type::Button, GetRoot());
		Button* button = dynamic_cast<Button*>(widget);
		button->m_Position = glm::vec2(100.0f, 100.0f);
		button->m_Size = glm::vec2(50.0f, 50.0f);
		button->m_BackgroundColor = glm::vec4(1.0f, 0.5f, 0.3f, 0.5f);
		button->m_Text = "Proba123";
		//button->m_Image = "awesomeface.png";
		//button->SetCallback([]() {std::cout << "clicked!\n"; });
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

	void MainMenu::SayHi()
	{
		std::cout << "Hi!\n";
	}



	REGISTER_CLASS(MainMenu, __FILE__)

}