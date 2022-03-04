#include "pch.h"
#include "Widget.h"
#include "Button.h"
#include "glad/glad.h""
#include "nanovg/nanovg.h"
#include "Core/FileSystem.h"



namespace Yugo
{

    Button::Button(Widget* parent)
        : Widget(parent)
    {
        m_Type = Widget::Type::Button;
    }

    void Button::Draw(NVGcontext* ctx)
    {
        nvgBeginPath(ctx);

        //float bounds[4];
        //nvgTextBounds(ctx, m_Position.x, m_Position.y, m_Text.c_str(), NULL, bounds);
        //nvgRoundedRect(ctx, bounds[0], bounds[1], bounds[2] - bounds[0], bounds[3] - bounds[1], m_EdgeRounding);
        nvgRoundedRect(ctx, m_Position.x, m_Position.y, m_Size.x, m_Size.y, m_EdgeRounding);
        nvgFillColor(ctx, nvgRGBAf(m_BackgroundColor.x, m_BackgroundColor.y, m_BackgroundColor.z, m_BackgroundColor.w));
        nvgFill(ctx);
        nvgStrokeColor(ctx, nvgRGBAf(m_EdgeColor.x, m_EdgeColor.y, m_EdgeColor.z, m_EdgeColor.w));
        nvgStroke(ctx);

        if (m_ImageFilePath != "")
        {
            int imageHandle = nvgCreateImage(ctx, m_ImageFilePath.c_str(), 0);
            NVGpaint paint = nvgImagePattern(ctx, m_Position.x, m_Position.y, m_Size.x, m_Size.y, 0, imageHandle, 1);
            nvgFillPaint(ctx, paint);
            nvgFill(ctx);
        }

        if (m_Text != "")
        {
            nvgFontSize(ctx, 17.0f);
            nvgFontFace(ctx, "sans");
            nvgFillColor(ctx, nvgRGBA(255, 255, 255, 160));
            nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
            nvgText(ctx, m_Position.x + m_Size.x / 2, m_Position.y, m_Text.c_str(), NULL);
        }

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

    void Button::Update(TimeStep ts)
    {
    }

    void Button::OnMouseButtonClick(Yugo::MouseCode button)
    {
        if (button == MOUSE_BUTTON_LEFT)
            m_OnClick();
    }

    void Button::SetImage(NVGcontext* ctx, const std::string& fileName)
    {
        std::string solutionFilePath = Yugo::FileSystem::GetSolutionFolderPath();
        m_ImageFilePath = solutionFilePath + "\\Main\\src\\Assets\\Textures\\UI" + fileName;
    }

}