#pragma once
#include "Core/Core.h"

class NVGcontext;
namespace Yugo
{

	class Widget
	{
	public:
		Widget(Widget* parent = nullptr);
		virtual ~Widget();

		virtual void Draw(NVGcontext* ctx) = 0;
		virtual void Update(TimeStep ts) = 0;

		virtual void OnLeftMouseClick() {}
		virtual void OnRightMouseClick() {}
		virtual void OnMouseHover() {}

		void SetParent(Widget* parent);
		void AddChild(Widget* child);

		std::vector<Widget*>& GetChildren();
		Widget* GetParent();

		glm::vec2 m_Position = glm::vec2(50);
		glm::vec2 m_Size = glm::vec2(50);
		glm::vec4 m_BackgroundColor = glm::vec4(1.0f);
		glm::vec4 m_EdgeColor = glm::vec4(1.0f);
		float m_EdgeThickness = 1.0f;
		
		bool m_IsVisible = true;

	protected:
		Widget* m_Parent = nullptr;
		std::vector<Widget*> m_Children;
		std::string m_UniqueId;

	};

}