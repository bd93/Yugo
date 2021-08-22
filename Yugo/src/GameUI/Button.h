#pragma once
#include "Widget.h"


namespace Yugo
{

	class Button : public Widget
	{
		void Draw(NVGcontext* ctx) override;
		void Update(TimeStep ts) override;

		void OnMouseLeftClick(Callback callback) override;

		float m_EdgeRounding = 0.0f;
	};

}