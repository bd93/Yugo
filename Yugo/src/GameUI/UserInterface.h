#pragma once
#include "Events/Event.h"
#include "Core/Core.h"
#include "Camera/Camera.h"

class NVGcontext;
namespace Yugo
{

	class UserInterfaceTree;
	class Widget;
	class UserInterface
	{

	public:
		UserInterface();
		~UserInterface();

		void OnStart();
		void OnEvent(const Event& event);
		void OnUpdate(TimeStep ts);
		void OnRender();
		void OnShutdown();

		void AddWidget(Widget* widget, Widget* parent);
		Widget* GetRoot();

		int m_FramebufferWidth;
		int m_FramebufferHeight;

	private:
		NVGcontext* m_NVGContext;
		UserInterfaceTree* m_UserInterfaceTree;
	};

}