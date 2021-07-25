#include "pch.h"
#include "Widget.h"



namespace Yugo
{

    Widget::Widget(Widget* parent)
    {
        if (parent)
            m_Parent = parent;
    }

    Widget::~Widget()
    {
        if (m_Parent)
        {
            delete m_Parent;
            m_Parent = nullptr;
        }

        for (auto child : m_Children)
        {
            delete child;
            child = nullptr;
        }
    }

    void Widget::SetParent(Widget* parent)
    {
        m_Parent = parent;
    }

    void Widget::AddChild(Widget* child)
    {
        m_Children.push_back(child);
    }

    std::vector<Widget*>& Widget::GetChildren()
    {
        return m_Children;
    }

    Widget* Widget::GetParent()
    {
        return m_Parent;
    }

}