#include "pch.h"
#include "Core/Dispatcher.h"
#include "UserInterface.h"
#include "Core/FileSystem.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Renderer/SpriteRenderer.h"
#include "Renderer/TextRenderer.h"
#include "Core/Dispatcher.h"

#include "UserInterfaceTree.h"
#include "Canvas.h" // TODO remove it later
#include "Widget.h"

#include <glad/glad.h>
#include "nanovg/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/nanovg_gl.h"


namespace Yugo
{

	UserInterface::UserInterface()
	{
		m_UserInterfaceTree = new UserInterfaceTree();

		Dispatcher::Subscribe<KeyboardKeyPress>(this);
		Dispatcher::Subscribe<MouseButtonPress>(this);
		Dispatcher::Subscribe<MouseScroll>(this);
	}

	UserInterface::~UserInterface()
	{
		delete m_UserInterfaceTree;
	}

	void UserInterface::OnStart()
	{
		//Shader uiTexturedShader(
		//	FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\ui_vertex.shader",
		//	FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\ui_textured_fragment.shader"
		//);
		//ResourceManager::AddShader("uiTexturedShader", uiTexturedShader);

		//Shader uiFlatColoredShader(
		//	FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\ui_vertex.shader",
		//	FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\ui_flat_colored_fragment.shader"
		//);
		//ResourceManager::AddShader("uiFlatColoredShader", uiFlatColoredShader);

		//Texture texture(
		//	FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Textures\\awesomeface.png"
		//);
		//ResourceManager::AddTexture("texture", texture);


		// nvg initialization
		m_NVGContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
		if (m_NVGContext == nullptr)
			throw std::runtime_error("Could not initialize NanoVG");

		// TODO: remove it later
		Canvas* canvas = new Canvas();
		canvas->SetParent(m_UserInterfaceTree->m_Root);
		canvas->m_Position = glm::vec2(100.0f, 100.0f);
		canvas->m_Size = glm::vec2(50.0f, 50.0f);
		canvas->m_BackgroundColor = glm::vec4(1.0f, 0.5f, 0.3f, 0.5f);
		AddWidget(canvas, m_UserInterfaceTree->m_Root);
	}

	void UserInterface::OnEvent(const Event& event)
	{
		if (event.GetEventType() == EventType::MouseButtonPress)
		{
			const auto& mouseButtonPress = static_cast<const MouseButtonPress&>(event);
			if (mouseButtonPress.GetButtonCode() == MOUSE_BUTTON_LEFT)
			{
				auto mousePosition = UserInput::GetMousePosition();
				Widget* intersectedWidget = m_UserInterfaceTree->CheckIntersectionWithMouse(mousePosition.first, mousePosition.second);
				if (intersectedWidget)
					intersectedWidget->OnLeftMouseClick();
			}
		}
	}

	void UserInterface::OnUpdate(TimeStep ts)
	{
		
	}

	void UserInterface::OnRender()
	{
		nvgBeginFrame(m_NVGContext, m_FramebufferWidth, m_FramebufferHeight, 1.f);
		m_UserInterfaceTree->Draw(m_NVGContext);
		nvgEndFrame(m_NVGContext);
	}

	void UserInterface::OnShutdown()
	{
	}

	void UserInterface::AddWidget(Widget* widget, Widget* parent)
	{
		// TODO: make it traverse...
		m_UserInterfaceTree->m_Root->AddChild(widget);
	}


}