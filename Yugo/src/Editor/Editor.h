#pragma once
#include "Scene/Scene.h"
#include "Core/Core.h"
#include "Events/Event.h"
#include "Core/Window.h"
#include "Scripting/ScriptEngine.h"
#include "GameUI/UserInterface.h"

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

namespace Yugo
{


	class Editor
	{
		friend class Application;

	public:
		Editor();

		void OnStart();
		void OnRender();
		void OnUpdate(TimeStep ts);
		void OnEvent(const Event& event);
		void OnShutdown();


	private:
		uPtr<Window> m_MainWindow;
		uPtr<Window> m_GameWindow; // Created as a second window when user clicks "Play Scene"
		sPtr<Scene> m_Scene;
		sPtr<UserInterface> m_UserInterface;
		uPtr<ScriptEngine> m_ScriptEngine;

		// Flags for Scene ImGui window inside editor
		bool m_IsSceneWindowFocused;
		bool m_IsSceneWindowHovered;

		entt::entity m_SelectedSceneEntity; // Entity that is selected on mouse left click

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

		// Mouse info relative to Scene Framebuffer (visible scene texture inside Scene ImGui window)
		struct MouseInfo
		{
			float MousePosX; 
			float MousePosY; 
			float MouseOffsetX;  
			float MouseOffsetY; 
		};
		MouseInfo m_MouseInfo;

		/*
		Active scene will be rendered in m_FrameBuffer.
		m_FrameBuffer will be used to display Scene inside Scene editor window.
		For anti-aliasing purpose intermediate framebuffer is used to display Scene inside Scene editor window, 
		because m_FrameBuffer has multisampled texture and only "normal" texture can be used.
		*/
		sPtr<FrameBuffer> m_FrameBuffer;
		sPtr<FrameBuffer> m_IntermediateFrameBuffer;
		sPtr<RenderBuffer> m_RenderBuffer;
		sPtr<Texture> m_Texture;
		sPtr<Texture> m_IntermediateTexture;

		void ShowMenuBar();
		void ShowProjectWindow();
		void ShowInspectorWindow(entt::registry& registry);
		void ShowHierarchyWindow(entt::registry& registry);
		void ShowSceneWindow();
		void ShowImGuizmoWidget(TransformComponent& transform, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
		void ShowFileDialogBox(const std::string& option, std::string& fullPath);
		void ShowFolderMenuPopup(const std::string& folderPath);
		void ShowHierarchyMenuPopup(entt::entity node, entt::registry& registry);

		void CreateWidget(const std::string& name, entt::entity parent);
		void CreateCanvasMatrixWidget(int dimensions[2], float padding[4], float cellWidgetSize[2], entt::entity parent);
		void CreateCopyEntity(entt::entity node);
		void CreateGameWindow();

		void CreateFrameBuffer(int width, int height);
		void BindFrameBuffer();
		void UnbindFrameBuffer();

		void ImportAsset(const std::string& importAssetFilePath);
		void SelectMesh();

		void UpdateSceneViewport(float sceneWindowWidth, float sceneWindowHeight);
	};

}