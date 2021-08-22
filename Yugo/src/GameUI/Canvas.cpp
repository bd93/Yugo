#include "pch.h"
#include "Canvas.h"
#include "glad/glad.h""
#include "nanovg/nanovg.h"

namespace Yugo
{

    Canvas::Canvas(Widget* parent)
        : Widget(parent)
    {
    }

    Canvas::~Canvas()
    {
    }

    void Canvas::Draw(NVGcontext* ctx)
    {
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, m_Position.x, m_Position.y, m_Size.x, m_Size.y, m_EdgeRounding);
        nvgFillColor(ctx, nvgRGBAf(m_BackgroundColor.x, m_BackgroundColor.y, m_BackgroundColor.z, m_BackgroundColor.w));
        nvgFill(ctx);
        nvgStrokeColor(ctx, nvgRGBAf(m_EdgeColor.x, m_EdgeColor.y, m_EdgeColor.z, m_EdgeColor.w));
        nvgStroke(ctx);

        //nvgSave(vg);
        //nvgTranslate(vg, 50, 50);
        //nvgBeginPath(vg);
        //nvgMoveTo(vg, 100 - 10, 100 + 2);
        //nvgLineTo(vg, 100 + 1, 100 + 2 - 11);
        //nvgLineTo(vg, 100 + 1, 100 + 2 + 11);
        //nvgLineTo(vg, 100 - 10, 100 + 2);
        //nvgStrokeColor(vg, nvgRGBA(200, 200, 200, 255));
        //nvgStroke(vg);
        //nvgRestore(vg);

    }

    void Canvas::Update(TimeStep ts)
    {
    }

    void Canvas::OnMouseLeftClick(Callback callback)
    {
        m_Callback = callback;
        //std::cout << "clicked!\n";
    }

}