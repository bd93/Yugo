#pragma once
#include "Scene/Scene.h"
#include "Core/Core.h"
#include "Events/Event.h"
#include "Core/Window.h"
#include "Scripting/ScriptEngine.h"

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

namespace Yugo
{


	class Editor
	{
	public:
		Editor();

		void OnStart();
		void OnRender();
		void OnUpdate(float ts);
		void OnEvent(const Event& event);
		void OnShutdown();


	private:
		// Flags
		bool m_IsSceneWindowFocused;
		bool m_IsSceneWindowHovered;

		// Scene Framebuffer info (visible scene texture inside Scene ImGui window)
		struct SceneInfo
		{
			std::string SceneName;
			std::string SceneFilePath;
			int SceneWidth;
			int SceneHeight;
			glm::vec2 ScenePosMin; // Upper left corner of Scene Framebuffer
			glm::vec2 ScenePosMax; // Lower right corner of Scene Framebuffer 
		};
		SceneInfo m_SceneInfo;

		// Relative to Scene Framebuffer
		struct MouseInfo
		{
			float MousePosX; 
			float MousePosY; 
			float MouseOffsetX;  
			float MouseOffsetY; 
		};
		MouseInfo m_MouseInfo;

		sPtr<Scene> m_Scene;
		uPtr<ScriptEngine> m_ScriptEngine;

		/*
		Active scene will be rendered in FrameBuffer;
		Frame buffer has a texture (color buffer) attachment
		and a render buffer as depth and stencil attachment;
		FrameBuffer will be used to display Scene inside Scene editor window
		*/
		sPtr<FrameBuffer> m_FrameBuffer;
		sPtr<RenderBuffer> m_RenderBuffer;
		sPtr<Texture> m_Texture;

		void ShowProjectWindow();
		void ShowInspectorWindow();
		void ShowHierarchyWindow();
		void ShowSceneWindow();
		void ShowImGuizmoWidget(TransformComponent& transform, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
		void ShowFileDialogBox(const std::string& option, std::string& fullPath);
		void ShowFolderMenuPopup(const std::string& folderPath);

		void CreateFrameBuffer(int width, int height);
		void BindFrameBuffer();
		void UnbindFrameBuffer();

		void UpdateSceneViewport(float sceneWindowWidth, float sceneWindowHeight);
	};

}