#pragma once
#include "Core/Core.h"

class NVGcontext;
namespace Yugo
{

	class Widget;
	class UserInterfaceTree
	{
		friend class UserInterface;
		using TraverseCallback = std::function<bool(Widget* widget)>;

	public:
		UserInterfaceTree();
		~UserInterfaceTree();

		void Update(TimeStep ts);
		void Draw(NVGcontext* ctx);

		/*
		* @brief This method scans tree and check if any widget is intersected with "mouse ray"
		* @return Widget pointer if there is intersection or nullptr if there is none
		*/
		Widget* CheckIntersectionWithMouse(float mousePosX, float mousePosY);

	private:
		Widget* m_Root; // Root node

		/*
		* @brief This method traverses tree down from fromWidget node and calls TraverseCallback for each node till the end
		*/
		void TraverseDownFrom(Widget* fromWidget, TraverseCallback callback);
		/*
		* @brief This method traverses tree up from fromWidget node and calls TraverseCallback for each node till root node
		*/
		void TraverseUpFrom(Widget* fromWidget, TraverseCallback callback);
		/*
		* @brief This method traverses tree down from fromWidget node and calls TraverseCallback for each node till toWidget node
		*/
		void TraverseDownFromTo(Widget* fromWidget, Widget* toWidget, TraverseCallback callback);
		/*
		* @brief This method traverses tree up from fromWidget node and calls TraverseCallback for each node till toWidget node
		*/
		void TraverseUpFromTo(Widget* fromWidget, Widget* toWidget, TraverseCallback callback);
	};

}