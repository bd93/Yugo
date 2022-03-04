#include "pch.h"
#include "UserInterfaceTree.h"
#include "Widget.h"
#include "Canvas.h"

#include "nanovg/nanovg.h"


namespace Yugo
{

	UserInterfaceTree::UserInterfaceTree()
	{
		m_Root = new Canvas();
		m_Root->m_IsVisible = false;
	}

	UserInterfaceTree::~UserInterfaceTree()
	{
		delete m_Root;
	}

	void UserInterfaceTree::Update(TimeStep ts)
	{
	}

	void UserInterfaceTree::Draw(NVGcontext* ctx)
	{
		TraverseDownFrom(m_Root, [&](Widget* widget) -> bool {
			if (widget->m_IsVisible)
				widget->Draw(ctx);
			return true;
		});
	}

	Widget* UserInterfaceTree::CheckIntersectionWithMouse(float mousePosX, float mousePosY)
	{
		Widget* intersectedWidget = nullptr;

		std::stack<Widget*> stack; // In case multiplt widgets overlap, top most element in a stack will be intersected Widget

		TraverseDownFrom(m_Root, [&](Widget* widget) -> bool {
			if (widget->m_IsVisible)
			{
				float edgeY1 = widget->m_Position.y;
				float edgeY2 = widget->m_Position.y + widget->m_Size.y;
				float edgeX1 = widget->m_Position.x;
				float edgeX2 = widget->m_Position.x + widget->m_Size.x;

				if (mousePosX > edgeX1 && mousePosX < edgeX2 && mousePosY > edgeY1 && mousePosY < edgeY2)
					stack.push(widget);
			}

			return true;
		});

		if (!stack.empty())
			intersectedWidget = stack.top();

		return intersectedWidget;
	}

	void UserInterfaceTree::TraverseDownFrom(Widget* fromWidget, TraverseCallback callback)
	{
		std::queue<Widget*> queue;

		if (!callback(fromWidget))
			return;
		queue.push(fromWidget);

		while (!queue.empty())
		{
			Widget* current = queue.front();
			queue.pop();

			for (auto child : current->GetChildren())
			{
				if (!callback(child))
					return;
				queue.push(child);
			}
		}
	}

	void UserInterfaceTree::TraverseUpFrom(Widget* fromWidget, TraverseCallback callback)
	{
	}

	void UserInterfaceTree::TraverseDownFromTo(Widget* fromWidget, Widget* toWidget, TraverseCallback callback)
	{
	}

	void UserInterfaceTree::TraverseUpFromTo(Widget* fromWidget, Widget* toWidget, TraverseCallback callback)
	{
	}

}