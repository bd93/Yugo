#pragma once
#include "Widget.h"


namespace Yugo
{
	class Button : public Widget
	{
	public:
		Button(Widget* parent = nullptr);

		void Draw(NVGcontext* ctx) override;
		void Update(TimeStep ts) override;

		void OnMouseButtonClick(Yugo::MouseCode button) override;
		void SetCallback(const std::function<void(void)>& callback) { m_OnClick = callback; }

		void SetImage(NVGcontext* ctx, const std::string& fileName);

		std::string m_Text = "";
		float m_EdgeRounding = 0.0f;

	private:
		std::function<void(void)> m_OnClick;
		std::string m_ImageFilePath = "";
	};

}