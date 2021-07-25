#include "pch.h"
#include "Editor.h"
#include "Animation/Components.h"
#include "Animation/Animation.h"
#include "Scripting/Components.h"
#include "Core/Time.h"
#include "Core/ModelImporter.h"
#include "Renderer/SpriteRenderer.h"
//#include "GameUI/Widget.h"

#include <ImGuizmo.h>
#include <windows.h>
#include <commdlg.h>
#include <entt/core/type_info.hpp>


namespace Yugo
{

	// Play mode flag
	static bool s_PlayMode = false;
	// Show/hide Scene in editor
	static bool s_RenderScene = true;
	// Show/hide in-game UI in editor
	static bool s_RenderUI = false;
	// Show/hide bounding boxes
	static bool s_RenderBoundingBox = false;

	// ImGuizmo widget data
	static ImGuizmo::OPERATION s_CurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE s_CurrentGizmoMode(ImGuizmo::WORLD);
	static bool s_UseSnap = false;
	static float s_Snap[3] = { 1.f, 1.f, 1.f };
	static float s_Bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float s_BoundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool s_BoundSizing = false;
	static bool s_BoundSizingSnap = false;

	Editor::Editor()
		: m_MainWindow(uPtrCreate<Window>("Editor", 1200, 800)),
		m_GameWindow(uPtrCreate<Window>("Game", 1200, 800)),
		m_Scene(sPtrCreate<Scene>()),
		m_UserInterface(sPtrCreate<UserInterface>()),
		m_ScriptEngine(uPtrCreate<ScriptEngine>()),
		m_SelectedSceneEntity(entt::null)
	{
		m_SceneInfo.SceneName = "Default";
		m_SceneInfo.SceneFilePath = "None";
		/*
		By default initialize width and heigh to main window size.
		In the next frame it will be resized to Scene ImGui window in UpdateSceneViewport method.
		*/
		m_SceneInfo.SceneWidth = m_MainWindow->m_Width;
		m_SceneInfo.SceneHeight = m_MainWindow->m_Height;
	}

	/**
	 * @brief Method to be called during application OnStart stage.
	 *
	 * This method initialize all necessary components.
	 */
	void Editor::OnStart()
	{
		m_MainWindow->OnStart();
		UserInput::SetGLFWwindow(m_MainWindow->m_GLFWwindow);

		// Initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(m_MainWindow->m_GLFWwindow, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Initially create texture framebuffer with default size (1200 x 800)
		CreateFrameBuffer(m_SceneInfo.SceneWidth, m_SceneInfo.SceneHeight);

		m_Scene->OnStart();
		//m_UserInterface->OnStart();
		//m_GameWindow->m_UserInterface->OnStart();

		Dispatcher::Subscribe<MouseButtonPress>(this);
		Dispatcher::Subscribe<KeyboardKeyPress>(this);
		Dispatcher::Subscribe<ImportAssetEvent>(this);
		Dispatcher::Subscribe<MouseScroll>(this);
		//Dispatcher::Subscribe<WindowResize>(this);
	}

	/**
	 * @brief Method to be called during application OnRender stage.
	 *
	 * This method renders ImGui widgets.
	 * Inside Scene window it renders Scene and UI.
	 */
	void Editor::OnRender()
	{
		// ImGui rendering initialisation
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		// OS specific window viewport
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Dock Space", NULL, window_flags);

		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

		/*
		The order of showing widgets/windows is important!
		E.g. ShowImGuizmo method, which is called in ShowSceneWindow method, need to be in front of ShowInspectorWindow. 
		The reason is because of model matrix update.
		Editor's OnUpdate method is called first and thus model matrix is updated with TransformComponent members (Position, Rotation, Scale).
		Then in ShowImGuizmo method model matrix is updated if user moves entity with imguizmo widget.
		Then in ShowInspectorWindow method model matrix is decomposed to TransformComponent members, so user can manualy change them in Inspector window.
		Just right after this model matrix is recomposed (updated) with TransformComponent members.
		*/
		ShowMenuBar();
		ShowSceneWindow();
		if (s_RenderScene)
		{
			ShowHierarchyWindow(m_Scene->m_Registry);
			ShowInspectorWindow(m_Scene->m_Registry);
		}
		else if (s_RenderUI)
		{
			//ShowHierarchyWindow(m_UserInterface->m_Registry);
			//ShowInspectorWindow(m_UserInterface->m_Registry);
		}
		ShowProjectWindow();

		ImGui::ShowDemoWindow();  // ImGui demo window with all possible widgets/features

		// Render Scene in Game window when editor is in play mode
		if (s_PlayMode)
		{
			Window::MakeContextCurrent(m_GameWindow->m_GLFWwindow);
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.3f, 0.3f, 0.3f, 1.0f); // Color of game window background
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//static bool init = false;
			//if (!init)
			//{
			//	m_GameWindow->m_UserInterface->OnStart();
			//	init = true;
			//}
			m_GameWindow->m_Scene->OnRender(); // Game window scene is a copy of Editor's scene
			m_GameWindow->m_UserInterface->OnRender();
			Window::PollEvents(); // temp!
			m_GameWindow->SwapBuffers();
			glDisable(GL_DEPTH_TEST);
			Window::MakeContextCurrent(m_MainWindow->m_GLFWwindow);
			
			if (m_GameWindow->WindowShouldClose())
			{
				s_PlayMode = false;
				m_ScriptEngine->OnStop();
				m_GameWindow->OnShutdown();
				UserInput::SetGLFWwindow(m_MainWindow->m_GLFWwindow);
				m_MainWindow->SetEventCallbacks();
				m_GameWindow->RemoveEventCallbacks();
			}
		}


		// Render Scene in editor's Scene window
		m_FrameBuffer->Bind();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f); // Color of framebuffer's texture inside scene imgui window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (s_RenderScene)
		{
			m_Scene->OnRender();

			if (s_RenderBoundingBox)
			{
				auto& camera = m_Scene->GetCamera();

				auto view = m_Scene->m_Registry.view<MeshComponent, BoundBoxComponent, TransformComponent>();
				for (auto entity : view)
				{
					auto& [aabb, transform] = view.get<BoundBoxComponent, TransformComponent>(entity);

					MeshRenderer::DrawAABB(aabb, transform, camera, ResourceManager::GetShader("quadShader"));

					//for (const auto& subAABB : aabb.SubAABBs)
					//{
					//	MeshRenderer::DrawAABB(subAABB, transform, ResourceManager::GetShader("quadShader"));
					//}
				}
			}

			glDisable(GL_DEPTH_TEST);
		}
		else if (s_RenderUI)
		{
			//m_UserInterface->OnRender();
		}

		m_FrameBuffer->BlitMultisampled(m_SceneInfo.SceneWidth, m_SceneInfo.SceneHeight, m_FrameBuffer->GetId(), m_IntermediateFrameBuffer->GetId());
		m_FrameBuffer->Unbind();

		// Render ImGui
		ImGuiIO& io = ImGui::GetIO();
		int glfwWindowWidth;
		int glfwWindowHeight;
		glfwGetWindowSize(m_MainWindow->m_GLFWwindow, &glfwWindowWidth, &glfwWindowHeight);
		io.DisplaySize = ImVec2((float)glfwWindowWidth, (float)glfwWindowHeight);

		ImGui::Render();

		/*
		Clear default framebuffer color and depth buffers which are attchaed to it;
		This is needed because scene is being rendered on custom framebuffer's attached texture
		and ImGUI is being renderd on default framebuffer
		*/
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Color of ImGui background (Main window)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Editor::OnUpdate(TimeStep ts)
	{
		if (s_PlayMode)
		{
			// The order of update is important! If scene is updated first, then script can't execute entity movement
			Window::MakeContextCurrent(m_GameWindow->m_GLFWwindow);
			m_ScriptEngine->OnUpdate(ts);
			m_GameWindow->m_Scene->OnUpdate(ts);
			m_GameWindow->m_UserInterface->OnUpdate(ts);
			Window::MakeContextCurrent(m_MainWindow->m_GLFWwindow);
		}
		else
		{
			if (s_RenderScene)
				m_Scene->OnUpdate(ts);
			else if (s_RenderUI)
				m_UserInterface->OnUpdate(ts);

			auto view = m_Scene->m_Registry.view<MeshComponent, TransformComponent, AnimationComponent>();
			for (auto entity : view)
			{
				auto& [mesh, animation] = view.get<MeshComponent, AnimationComponent>(entity);
				if (animation.IsAnimationRunning)
					Animation::RunAnimation(mesh, animation);
			}

			if (UserInput::IsKeyboardKeyPressed(KEY_LEFT_CONTROL) && UserInput::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_IsSceneWindowHovered)
			{
				auto view = m_Scene->m_Registry.view<CameraComponent, TransformComponent, EntityTagComponent>();
				for (auto entity : view)
				{
					auto& [camera, transform, tag] = view.get<CameraComponent, TransformComponent, EntityTagComponent>(entity);
					if (tag.Name == "Main Camera")
						Camera::RotateAroundPivot(transform, camera);
				}
			}
		}
	}

	void Editor::OnEvent(const Event& event)
	{
		if (s_PlayMode)
		{
			Window::MakeContextCurrent(m_GameWindow->m_GLFWwindow);
			m_ScriptEngine->OnEvent(event);

			if (event.GetEventType() == EventType::WindowResize)
			{
				// TODO: Check if Projection Matrix should be updated 
				//const auto& windowResize = static_cast<const WindowResize&>(event);
				//int screenWidth = windowResize.GetWidth();
				//int screenHeight = windowResize.GetHeight();
				//auto& camera = m_Scene->GetCamera();
				//Camera::UpdateProjectionMatrix(camera, screenWidth, screenHeight);
			}

			Window::MakeContextCurrent(m_MainWindow->m_GLFWwindow);
		}
		else
		{
			// Cast mouse ray, Select entity, reset mouse position offset for camera
			if (event.GetEventType() == EventType::MouseButtonPress)
			{
				const auto& mouseButtonPress = static_cast<const MouseButtonPress&>(event);
				if (mouseButtonPress.GetButtonCode() == MOUSE_BUTTON_LEFT)
				{
					if (!ImGuizmo::IsOver(s_CurrentGizmoOperation) && m_IsSceneWindowHovered)
					{
						SelectMesh();
						Camera::ResetMousePositionOffset(m_Scene->GetCamera());
					}
				}
			}

			// Import asset event is invoked when user drag and drop asset to scene imgui window 
			if (event.GetEventType() == EventType::ImportAsset)
			{
				auto& camera = m_Scene->GetCamera();
				MouseRay::CalculateRayOrigin(camera, m_MouseInfo.MousePosX, m_MouseInfo.MousePosY, m_SceneInfo.SceneWidth, m_SceneInfo.SceneHeight);
				
				const auto& importAssetEvent = static_cast<const ImportAssetEvent&>(event);
				const auto& importAssetFilePath = importAssetEvent.GetAssetFilePath();

				ImportAsset(importAssetFilePath);
			}

			// Camera zoom in / zoom out
			if (event.GetEventType() == EventType::MouseScroll)
			{
				if (m_IsSceneWindowHovered)
				{
					const auto& mouseScroll = static_cast<const MouseScroll&>(event);
					auto& camera = m_Scene->GetCamera();
					Camera::Scroll(mouseScroll.GetOffsetY(), camera);
				}
			}
		}
	}

	void Editor::OnShutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		//m_ScriptEngine->OnShutdown();
		m_Scene->OnShutdown();
		m_MainWindow->OnShutdown();
		Window::TerminateGLFW();
	}

	void Editor::ShowMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			static bool saveSuccess = false;
			static bool saveError = false;

			if (saveSuccess)
			{
				ImGui::OpenPopup("Success!");

				if (ImGui::BeginPopupModal("Success!"))
				{
					ImGui::Text("This scene has been saved");
					ImGui::Separator();
					if (ImGui::Button("OK", ImVec2(120, 0))) { saveSuccess = false;  ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
			}

			if (saveError)
			{
				ImGui::OpenPopup("Error!");

				if (ImGui::BeginPopupModal("Error!"))
				{
					ImGui::Text("First click Save As...");
					ImGui::Separator();
					if (ImGui::Button("OK", ImVec2(120, 0))) { saveError = false;  ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
			}

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene"))
				{
					m_Scene->m_Registry.clear();
					m_SceneInfo.SceneName = "Default";
					m_SceneInfo.SceneFilePath = "None";
				}

				if (ImGui::MenuItem("Load Scene"))
				{
					std::string filePath;
					ShowFileDialogBox("Open", filePath);
					m_Scene->LoadScene(filePath);
					m_SceneInfo.SceneFilePath = filePath;
					std::size_t foundBegin = filePath.find_last_of("\\");
					std::size_t foundEnd = filePath.find_last_of(".");
					std::string fileName = filePath.substr(foundBegin + 1, foundEnd - foundBegin - 1);
					m_SceneInfo.SceneName = fileName;
				}
				ImGui::Separator();

				if (ImGui::MenuItem("Save"))
				{
					const auto& filePath = m_SceneInfo.SceneFilePath;

					if (filePath == "None")
					{
						saveError = true;
					}
					else
					{
						m_Scene->SaveScene(filePath);
						saveSuccess = true;
					}
				}

				if (ImGui::MenuItem("Save As..."))
				{
					std::string filePath;
					ShowFileDialogBox("Save As", filePath);
					m_Scene->SaveScene(filePath);
					m_SceneInfo.SceneFilePath = filePath;
					std::size_t foundBegin = filePath.find_last_of("\\");
					std::size_t foundEnd = filePath.find_last_of(".");
					std::string fileName = filePath.substr(foundBegin + 1, foundEnd - foundBegin - 1);
					m_SceneInfo.SceneName = fileName;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	void Editor::ShowProjectWindow()
	{
		using TraverseFun = std::function<void(const std::string&)>;

		ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Project"))
		{
			static std::string currentPath;

			// left
			ImGui::BeginChild("left pane", ImVec2(250, 0), true);

			static size_t selection = 0;
			int rootId = 1;
			static size_t nodeClicked = 0;

			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
			if (selection == rootId)
			{
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
				currentPath = FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets";
			}

			std::string pathToFolder = FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets";
			auto folderTreeMap = FileSystem::HashFolderTree(pathToFolder);

			bool rootNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)rootId, nodeFlags, "Assets");

			if (ImGui::IsItemClicked())
			{
				nodeClicked = rootId;
				currentPath = pathToFolder;
			}

			if (ImGui::IsItemClicked(1))
				ImGui::OpenPopup(pathToFolder.c_str());
			ShowFolderMenuPopup(pathToFolder);

			TraverseFun TraverseFolderTree = [&](const std::string& path) {

				for (const auto& entry : FileSystem::DirIter(path))
				{
					if (entry.is_directory())
					{
						std::string folderName = entry.path().filename().string();
						std::string pathToFolder = path + "\\" + folderName;

						ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
						if (selection == folderTreeMap[folderName])
							nodeFlags |= ImGuiTreeNodeFlags_Selected;

						bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)folderTreeMap[folderName], nodeFlags, "%s", folderName.c_str());

						if (ImGui::IsItemClicked())
						{
							nodeClicked = folderTreeMap[folderName];
							currentPath = pathToFolder;
						}

						if (ImGui::IsItemClicked(1))
							ImGui::OpenPopup(pathToFolder.c_str());
						ShowFolderMenuPopup(pathToFolder);

						if (nodeOpen)
						{
							TraverseFolderTree(pathToFolder);
							ImGui::TreePop();
						}
					}
					else
					{
						std::string fileName = entry.path().filename().string();
						std::string pathToFile = path + "\\" + fileName;

						ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
						if (selection == folderTreeMap[fileName])
							nodeFlags |= ImGuiTreeNodeFlags_Selected;

						ImGui::TreeNodeEx((void*)(intptr_t)folderTreeMap[fileName], nodeFlags, "%s", fileName.c_str());
						if (ImGui::IsItemClicked())
						{
							nodeClicked = folderTreeMap[fileName];
						}
						if (ImGui::IsItemClicked(1))
						{
							ImGui::OpenPopup(pathToFile.c_str());
						}
						if (ImGui::BeginPopup(pathToFile.c_str()))
						{
							ImGui::Text("Are you sure you want to delete this file?");
							if (ImGui::Button("OK"))
							{
								FileSystem::Delete(pathToFile);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();
							if (ImGui::Button("Cancel"))
							{
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
					}
				}
			};


			if (rootNodeOpen)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()); // Increase spacing to differentiate leaves from expanded contents.
				TraverseFolderTree(FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets");

				if (nodeClicked != 0)
				{
					selection = nodeClicked;
				}

				ImGui::PopStyleVar();
				ImGui::TreePop();
			}

			ImGui::EndChild();
			ImGui::SameLine();

			// right
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			ImGui::Text(currentPath.c_str());
			ImGui::Separator();

			if (currentPath != "")
			{
				if (ImGui::Button("Add Asset"))
				{
					std::string filePath;
					ShowFileDialogBox("Open", filePath);
					std::string fileName = FileSystem::FilePath(filePath).filename().string();
					FileSystem::CopyFileTo(filePath, currentPath + "\\" + fileName);
				}
			}

			ImGui::Text("Drag and drop asset:");
			ImGui::Dummy(ImVec2(0.0f, 10.0f)); // Padding from top line separator
			int i = 0;
			if (currentPath != "")
			{
				ImGui::Columns(7);
				for (auto& entry : FileSystem::DirIter(currentPath))
				{
					if (!entry.is_directory())
					{
						std::string fileName = entry.path().filename().string();

						ImGui::PushID(i++);

						ImGui::Button(fileName.c_str(), ImVec2(-FLT_MIN, 0.0f));
						ImGui::NextColumn();

						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
						{
							ImGui::SetDragDropPayload("Import Asset", (currentPath + "\\" + fileName).c_str(), (currentPath + "\\" + fileName).size() + 1);
							ImGui::Text("Drag and drop %s", fileName.c_str());
							ImGui::EndDragDropSource();
						}
						ImGui::PopID();
					}
				}
			}

			ImGui::EndChild();
			ImGui::EndGroup();
		}
		ImGui::End();
	}

	void Editor::ShowInspectorWindow(entt::registry& registry)
	{
		ImGui::Begin("Inspector");

		static int radioOption = 0;
		static int pastRadiooption = radioOption;
		ImGui::RadioButton("Show Scene", &radioOption, 0); 
		ImGui::SameLine();
		ImGui::RadioButton("Show UI", &radioOption, 1);
		if (radioOption == 0)
		{
			s_RenderScene = true;
			s_RenderUI = false;
		}
		else if (radioOption == 1)
		{
			s_RenderScene = false;
			s_RenderUI = true;
		}
		if (pastRadiooption != radioOption)
		{
			m_SelectedSceneEntity = entt::null;
			s_CurrentGizmoOperation = ImGuizmo::BOUNDS;
			pastRadiooption = radioOption;
		}

		ImGui::Separator();

		const char* components[] = { "MeshComponent", "SpriteComponent", "AnimationComponent", "ScriptComponent" };
		static bool toggles[] = { false, false, false, false }; // For toggle widget

		if (m_SelectedSceneEntity != entt::null)
		{
			for (int i = 0; i < IM_ARRAYSIZE(components); i++)
			{
				toggles[i] = false; // Reset previous state
				if (components[i] == "MeshComponent" && registry.has<MeshComponent>(m_SelectedSceneEntity))
					toggles[i] = true;
				if (components[i] == "SpriteComponent" && registry.has<SpriteComponent>(m_SelectedSceneEntity))
					toggles[i] = true;
				if (components[i] == "AnimationComponent" && registry.has<AnimationComponent>(m_SelectedSceneEntity))
					toggles[i] = true;
				if (components[i] == "ScriptComponent" && registry.has<ScriptComponent>(m_SelectedSceneEntity))
					toggles[i] = true;
			}

			// Always show TransformComponent
			auto& transform = registry.get<TransformComponent>(m_SelectedSceneEntity);

			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform.ModelMatrix), glm::value_ptr(transform.Position), glm::value_ptr(transform.Rotation), glm::value_ptr(transform.Scale));
			ImGui::InputFloat3("Translate", glm::value_ptr(transform.Position));
			ImGui::InputFloat3("Rotate", glm::value_ptr(transform.Rotation));
			ImGui::InputFloat3("Scale", glm::value_ptr(transform.Scale));
			ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(transform.Position), glm::value_ptr(transform.Rotation), glm::value_ptr(transform.Scale), glm::value_ptr(transform.ModelMatrix));
			
			ImGui::NewLine();

			// Show padding options for UI canvas widget
			//if (registry.has<CanvasWidgetComponent>(m_SelectedSceneEntity))
			//{
			//	//auto& relationship = registry.get<RelationshipComponent>(m_SelectedSceneEntity);
			//	static float padding[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // left, right, top, bottom
			//	ImGui::InputFloat4("Padding", padding);
			//	static int dimensions[2] = { 1, 1 };
			//	ImGui::InputInt2("Rows x Columns", dimensions);
			//	static float cellWidgetSize[2] = { 50.0f, 50.0f };
			//	ImGui::InputFloat2("Cell Widget Size", cellWidgetSize);
			//	if (ImGui::Button("Create")) // Create canvas matrix widget
			//		CreateCanvasMatrixWidget(dimensions, padding, cellWidgetSize, m_SelectedSceneEntity);
			//}
			//// Show text input for widget's text
			//if (registry.has<TextWidgetComponent>(m_SelectedSceneEntity))
			//{
			//	static entt::entity previousEntity;
			//	static char buf[64] = "";
			//	auto& text = registry.get<TextWidgetComponent>(m_SelectedSceneEntity);
			//	if (previousEntity != m_SelectedSceneEntity)
			//		memset(buf, 0, 64 * sizeof(char));
			//	if (ImGui::InputText("Text", buf, 64))
			//		text.Text = std::string(buf);
			//	previousEntity = m_SelectedSceneEntity;
			//	ImGui::NewLine();
			//}

			if (ImGui::RadioButton("Local", s_CurrentGizmoMode == ImGuizmo::LOCAL))
				s_CurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", s_CurrentGizmoMode == ImGuizmo::WORLD))
				s_CurrentGizmoMode = ImGuizmo::WORLD;

			ImGui::NewLine();

			if (ImGui::IsKeyPressed((int)KEY_S))
				s_UseSnap = !s_UseSnap;
			
			ImGui::PushID(0);
			ImGui::Checkbox("", &s_UseSnap);
			ImGui::PopID();
			ImGui::SameLine();

			switch (s_CurrentGizmoOperation)
			{
			case ImGuizmo::TRANSLATE:
				ImGui::InputFloat3("Snap", &s_Snap[0]);
				break;
			case ImGuizmo::ROTATE:
				ImGui::InputFloat("Angle Snap", &s_Snap[1]);
				break;
			case ImGuizmo::SCALE:
				ImGui::InputFloat("Scale Snap", &s_Snap[2]);
				break;
			}

			ImGui::Checkbox("Bound Sizing", &s_BoundSizing);
			if (s_BoundSizing)
			{
				ImGui::PushID(1);
				ImGui::Checkbox("", &s_BoundSizingSnap);
				ImGui::SameLine();
				ImGui::InputFloat3("Snap", s_BoundsSnap);
				ImGui::PopID();
			}

			ImGui::Separator();

			for (int i = 0; i < IM_ARRAYSIZE(components); ++i)
			{
				if (toggles[i])
				{
					if (components[i] == "MeshComponent")
					{
						ImGui::Checkbox("Show Bounding Box", &s_RenderBoundingBox);

						ImGui::Separator();
					}

					if (components[i] == "AnimationComponent")
					{
						auto view = registry.view<MeshComponent, AnimationComponent>();
						for (auto entity : view)
						{
							auto& [mesh, animation] = view.get<MeshComponent, AnimationComponent>(entity);
							if (entity == m_SelectedSceneEntity)
							{
								static std::string firstAnimationName = "None"; // Keep track of selected entity's first animation name in order to show it in Combo preview
								static std::string currentAnimationName = animation.AnimationNameVec[0];
								if (firstAnimationName != animation.AnimationNameVec[0])
								{
									firstAnimationName = animation.AnimationNameVec[0];
									currentAnimationName = firstAnimationName;
								}

								if (ImGui::BeginCombo("Animation Names", currentAnimationName.c_str())) // The second parameter is the label previewed before opening the combo.
								{
									for (uint32_t i = 0; i < animation.AnimationNameVec.size(); ++i)
									{
										bool isSelected = (currentAnimationName == animation.AnimationNameVec[i]);
										if (ImGui::Selectable(animation.AnimationNameVec[i].c_str(), isSelected))
											currentAnimationName = animation.AnimationNameVec[i];
										if (isSelected)
											ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo
									}
									ImGui::EndCombo();
								}

								if (ImGui::Button("Start Animation"))
								{
									animation.RunningAnimationName = currentAnimationName;
									animation.IsAnimationRunning = true;
									animation.AnimationMap[currentAnimationName].AnimationStartingTime = Time::CurrentRealTime();
								}

								ImGui::SameLine();

								if (ImGui::Button("Stop Animation"))
								{
									animation.IsAnimationRunning = false;
									animation.RunningAnimationName = "None";
								}
							}
						}

						ImGui::Separator();
					}

					if (components[i] == "ScriptComponent")
					{
						static std::string scriptFilePath = "Script: None";
						static entt::entity previousEntity = m_SelectedSceneEntity;

						if (previousEntity != m_SelectedSceneEntity)
						{
							if (registry.has<ScriptComponent>(m_SelectedSceneEntity))
							{
								auto& scriptComponent = registry.get<ScriptComponent>(m_SelectedSceneEntity);
								scriptFilePath = scriptComponent.ScriptFilePath;
							}
							else
							{
								scriptFilePath = "Script: None";
							}
							previousEntity = m_SelectedSceneEntity;
						}
						ImGui::Text(scriptFilePath.c_str());

						if (ImGui::Button("Select Script"))
							ShowFileDialogBox("Open", scriptFilePath);

						ImGui::SameLine();
						if (ImGui::Button("Attach Script"))
						{
							auto& scriptComponent = registry.get<ScriptComponent>(m_SelectedSceneEntity);
							auto& entityTagComponent = registry.get<EntityTagComponent>(m_SelectedSceneEntity);

							scriptComponent.ScriptFilePath = scriptFilePath;
							//Entity entity(m_SelectedSceneEntity, entityTagComponent.Name, m_Scene.get());
							//m_ScriptEngine->AttachScript(scriptFilePath, entity);
							m_ScriptEngine->AttachScript(scriptFilePath, m_SelectedSceneEntity);
						}

						ImGui::Separator();
					}

					if (components[i] == "SpriteComponent")
					{
						auto view = registry.view<SpriteComponent>();
						if (m_SelectedSceneEntity != entt::null)
						{
							auto& sprite = registry.get<SpriteComponent>(m_SelectedSceneEntity);
							auto& texture = sprite.Texture;
							ImGui::Text("Texture:");
							ImGui::Image((void*)texture.GetId(), ImVec2(80.0f, 80.0f), ImVec2(0, 0), ImVec2(1, 1));
							if (ImGui::BeginDragDropTarget())
							{
								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Import Asset"))
								{
									char* payloadData = (char*)payload->Data;
									std::string assetFilePath(payloadData);
									if (ResourceManager::HasTexture(assetFilePath))
									{
										texture = ResourceManager::GetTexture(assetFilePath);
									}
									else
									{
										ResourceManager::AddTexture(assetFilePath, Texture(assetFilePath));
										texture = ResourceManager::GetTexture(assetFilePath);
									}
									sprite.HasTexture = true;
								}
								ImGui::EndDragDropTarget();
							}

							ImGui::Separator();

							ImGui::Text("Color:");
							static ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
							static ImVec4 ref_color_v(1.0f, 1.0f, 1.0f, 1.0f);
							ImGuiColorEditFlags flags =
								ImGuiColorEditFlags_AlphaPreview |
								ImGuiColorEditFlags_AlphaBar |
								ImGuiColorEditFlags_DisplayRGB |
								ImGuiColorEditFlags_DisplayHex;
							if (ImGui::ColorPicker4("My Color", (float*)&color, flags, &ref_color_v.x))
							{
								//if (registry.has<TextWidgetComponent>(m_SelectedSceneEntity))
								//{
								//	auto& text = registry.get<TextWidgetComponent>(m_SelectedSceneEntity);
								//	text.Color = glm::vec4(color.x, color.y, color.z, color.w);
								//}
								//else
								//{
								//	sprite.Color = glm::vec4(color.x, color.y, color.z, color.w);
								//}
							}

							ImGui::Separator();
						}
					}
				}
			}

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("addComponentPopup");
			ImGui::SameLine();
			if (ImGui::BeginPopup("addComponentPopup"))
			{
				for (int i = 0; i < IM_ARRAYSIZE(components); i++)
				{
					if (ImGui::MenuItem(components[i], "", &toggles[i]))
					{
						if (components[i] == "MeshComponent")
						{
							if (toggles[i])
							{
								auto& entityTag = registry.get<EntityTagComponent>(m_SelectedSceneEntity);
								auto& [loadedMesh, loadedAnimation] = ModelImporter::LoadMeshFile(entityTag.AssetFilePath);
								auto& mesh = registry.emplace<MeshComponent>(m_SelectedSceneEntity, *loadedMesh);
								MeshRenderer::Submit(mesh);
							}
							else
							{
								registry.remove<MeshComponent>(m_SelectedSceneEntity);
							}
						}
						if (components[i] == "SpriteComponent")
						{
							if (toggles[i])
							{
								auto& sprite = registry.emplace<SpriteComponent>(m_SelectedSceneEntity);
								SpriteRenderer::Submit(sprite);
							}
							else
							{
								registry.remove<SpriteComponent>(m_SelectedSceneEntity);
							}
						}
						if (components[i] == "AnimationComponent")
						{
							if (toggles[i])
								registry.emplace<AnimationComponent>(m_SelectedSceneEntity);
							else
								registry.remove<AnimationComponent>(m_SelectedSceneEntity);
						}
						if (components[i] == "ScriptComponent")
						{
							if (toggles[i])
							{
								registry.emplace<ScriptComponent>(m_SelectedSceneEntity);
							}
							else
							{
								registry.remove<ScriptComponent>(m_SelectedSceneEntity);
							}
						}
					}
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void Editor::ShowHierarchyWindow(entt::registry& registry)
	{
		if (ImGui::Begin("Hierarchy", NULL))
		{
			using TraverseFun = std::function<void(entt::entity)>;

			static uint32_t selection = 0;
			uint32_t rootId = 1;
			static uint32_t nodeClicked = 0;

			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
			if (selection == rootId)
				nodeFlags |= ImGuiTreeNodeFlags_Selected;

			const char* sceneName;
			sceneName = m_SceneInfo.SceneName.c_str();

			bool rootNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)rootId, nodeFlags, sceneName);

			if (ImGui::IsItemClicked())
			{
				nodeClicked = rootId;
				m_SelectedSceneEntity = entt::null;
			}

			if (ImGui::IsItemClicked(1))
				ImGui::OpenPopup(std::to_string(rootId).c_str());
			ShowHierarchyMenuPopup(entt::null, registry);

			auto view = registry.view<RelationshipComponent, EntityTagComponent>();

			TraverseFun traverse = [&](entt::entity entity) {

				auto& tag = view.get<EntityTagComponent>(entity);
				/*
				Bug is triggered in the following situation:
				If I get relationship by reference, then there shouldn't be any new entity added to the registry.
				If it isn't the case, then relationship reference has trash values for parent entity as well as number of children value;
				In the line "ShowHierarchyMenuPopup(entity);" a new entity is added, thus registry is changed;
				Because of that I return relationship by value (anyway I don't have to modify relationship in this method);
				*/
				auto relationship = view.get<RelationshipComponent>(entity);
				if (relationship.NumOfChildren == 0) // Leaf node
				{
					ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

					const char* entityTag;
					entityTag = tag.Name.c_str();
					uint32_t entityId = rootId + (uint32_t)entity + 1;

					if (selection == entityId)
						nodeFlags |= ImGuiTreeNodeFlags_Selected;
					ImGui::TreeNodeEx((void*)(uintptr_t)entityId, nodeFlags, entityTag);

					if (ImGui::IsItemClicked())
					{
						m_SelectedSceneEntity = entity;
						if (s_CurrentGizmoOperation == ImGuizmo::BOUNDS)
							s_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
					}

					if (ImGui::IsItemClicked(1))
						ImGui::OpenPopup(std::to_string(entityId).c_str());
					ShowHierarchyMenuPopup(entity, registry);
				}
				else
				{
					ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
					uint32_t entityId = rootId + (uint32_t)entity + 1;

					if (selection == entityId)
						nodeFlags |= ImGuiTreeNodeFlags_Selected;

					const char* entityTag;
					entityTag = tag.Name.c_str();

					bool rootNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entityId, nodeFlags, entityTag);

					if (ImGui::IsItemClicked())
					{
						m_SelectedSceneEntity = entity;
						if (s_CurrentGizmoOperation == ImGuizmo::BOUNDS)
							s_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
					}

					if (ImGui::IsItemClicked(1))
						ImGui::OpenPopup(std::to_string(entityId).c_str());
					ShowHierarchyMenuPopup(entity, registry);

					if (rootNodeOpen)
					{
						auto relationship = view.get<RelationshipComponent>(entity);
						for (auto child : relationship.Children)
							traverse(child);
						ImGui::TreePop();
					}
				}
			};

			if (rootNodeOpen)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()); // Increase spacing to differentiate leaves from expanded contents.

				auto view = registry.view<RelationshipComponent, EntityTagComponent, TransformComponent>();
				for (auto entity : view)
				{
					auto& relationship = view.get<RelationshipComponent>(entity);
					if (relationship.Parent == entt::null) // Recursive traverse only if entity's parent is root node (entt::null)
						traverse(entity);
				}

				// Nodes will be selected either by clicking on it or by clicking on model in scene
				if (m_SelectedSceneEntity != entt::null)
					nodeClicked = (uint32_t)m_SelectedSceneEntity + 2; // 0 and 1 are already reserved => Id = 0 for entt::null, Id = 1 for root node (scene name)
				else
					if (nodeClicked != rootId) nodeClicked = 0;

				selection = nodeClicked;

				ImGui::PopStyleVar();
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

	void Editor::ShowSceneWindow()
	{
		ImGui::Begin("Scene", NULL);

		m_IsSceneWindowFocused = ImGui::IsWindowFocused();

		/*
		Keep track of Scene Window's width and height;
		If scene window is resized, then Framebuffer (where scene is rendered) needs to be resized too
		*/
		float sceneWindowWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
		float sceneWindowHeight = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;
		UpdateSceneViewport(sceneWindowWidth, sceneWindowHeight); // Update Framebuffer's size and viewport

		// xPadding and yPadding are distances between scene framebuffer texture and scene ImGui window;
		float xPadding = (ImGui::GetWindowSize().x - m_SceneInfo.SceneWidth) * 0.5f;
		float yPadding = (ImGui::GetWindowSize().y + ImGui::GetItemRectSize().y - m_SceneInfo.SceneHeight) * 0.5f;

		ImGui::SetCursorPos(ImVec2(xPadding, yPadding));
		//ImGui::Image((void*)m_Texture->GetId(), ImVec2((float)m_SceneInfo.SceneWidth, (float)m_SceneInfo.SceneHeight), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((void*)m_IntermediateTexture->GetId(), ImVec2((float)m_SceneInfo.SceneWidth, (float)m_SceneInfo.SceneHeight), ImVec2(0, 1), ImVec2(1, 0)); // temp
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Import Asset"))
			{
				char* payloadData = (char*)payload->Data;
				std::string strPayloadData(payloadData);
				ImportAssetEvent importAssetEvent(strPayloadData);
				Dispatcher::Publish(importAssetEvent);
			}
			ImGui::EndDragDropTarget();
		}

		float scenePosMinX = ImGui::GetWindowPos().x + xPadding;
		float scenePosMinY = ImGui::GetWindowPos().y + yPadding;

		float scenePosMaxX = scenePosMinX + m_SceneInfo.SceneWidth;
		float scenePosMaxY = scenePosMinY + m_SceneInfo.SceneHeight;

		ImVec2 scenePosMin = ImVec2(scenePosMinX, scenePosMinY);
		ImVec2 scenePosMax = ImVec2(scenePosMaxX, scenePosMaxY);

		// Relative to Scene Framebuffer
		m_SceneInfo.ScenePosMin = glm::vec2(scenePosMinX, scenePosMinY);
		m_SceneInfo.ScenePosMax = glm::vec2(scenePosMaxX, scenePosMaxY);
		
		// Relative to Scene Framebuffer
		m_MouseInfo.MousePosX = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - xPadding;
		m_MouseInfo.MousePosY = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - yPadding;

		if (ImGui::IsMouseHoveringRect(scenePosMin, scenePosMax))
			m_IsSceneWindowHovered = true;
		else
			m_IsSceneWindowHovered = false;

		if (m_SelectedSceneEntity != entt::null)
		{
			if (s_RenderScene)
			{
				auto& transform = m_Scene->m_Registry.get<TransformComponent>(m_SelectedSceneEntity);
				auto& camera = m_Scene->GetCamera();
				ShowImGuizmoWidget(transform, camera.Projection, camera.View);
			}
			else if (s_RenderUI)
			{
				//auto& transform = m_UserInterface->m_Registry.get<TransformComponent>(m_SelectedSceneEntity);
				//auto& camera = m_UserInterface->GetCamera();
				// 
				//ShowImGuizmoWidget(transform, camera.Projection, glm::mat4(1.0f));
			}
		}
		
		ImGui::End();

		ImGui::Begin("Scene Commands");

		if (ImGui::Button("Play Scene"))
		{
			s_PlayMode = true;
			if (m_SelectedSceneEntity != entt::null)
				m_SelectedSceneEntity = entt::null;

			CreateGameWindow();
			m_ScriptEngine->OnStart(m_GameWindow->m_Scene.get(), m_GameWindow.get());
		}

		ImGui::SameLine();
		
		if (ImGui::Button("Stop Scene"))
		{
			s_PlayMode = false;
			m_ScriptEngine->OnStop();
			m_GameWindow->OnShutdown();
			UserInput::SetGLFWwindow(m_MainWindow->m_GLFWwindow);
		}
		
		ImGui::End();
	}

	void Editor::ShowImGuizmoWidget(TransformComponent& transform, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
	{
		ImGuizmo::BeginFrame();
		ImGuizmo::SetOrthographic(true);

		if (ImGui::IsKeyDown((int)KEY_LEFT_CONTROL) && ImGui::IsKeyPressed((int)KEY_T))
			s_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyDown((int)KEY_LEFT_CONTROL) && ImGui::IsKeyPressed((int)KEY_R))
			s_CurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyDown((int)KEY_LEFT_CONTROL) && ImGui::IsKeyPressed((int)KEY_Y))
			s_CurrentGizmoOperation = ImGuizmo::SCALE;

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(
			m_SceneInfo.ScenePosMin.x,
			m_SceneInfo.ScenePosMin.y,
			(float)m_SceneInfo.SceneWidth,
			(float)m_SceneInfo.SceneHeight
		);
		ImGuizmo::Manipulate(
			glm::value_ptr(viewMatrix),
			glm::value_ptr(projectionMatrix),
			s_CurrentGizmoOperation,
			s_CurrentGizmoMode,
			glm::value_ptr(transform.ModelMatrix),
			NULL,
			s_UseSnap ? &s_Snap[0] : NULL,
			s_BoundSizing ? s_Bounds : NULL,
			s_BoundSizingSnap ? s_BoundsSnap : NULL
		);

		// Update delta position and matrix components (position, rotation, scale) after guizmo manipulation
		float position[3];
		float rotation[3];
		float scale[3];
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform.ModelMatrix), position, rotation, scale);
		auto deltaPosition = glm::vec3(position[0] - transform.Position.x, position[1] - transform.Position.y, position[2] - transform.Position.z); // Capture the amount by which this widget has been moved

		// Update delta position for all children
		if (s_RenderScene)
		{
			auto view = m_Scene->m_Registry.view<RelationshipComponent, TransformComponent>();
			using TraverseFun = std::function<void(entt::entity)>;
			TraverseFun traverse = [&](entt::entity entity) {
				auto& relationship = view.get<RelationshipComponent>(entity);
				for (auto child : relationship.Children)
				{
					auto& childTransform = view.get<TransformComponent>(child);
					//childTransform.DeltaPosition = transform.DeltaPosition;
					childTransform.DeltaPosition = deltaPosition;
				}
				for (auto child : relationship.Children)
				{
					traverse(child);
				}
			};
			traverse(m_SelectedSceneEntity);
		}
		else if (s_RenderUI)
		{
			//auto view = m_UserInterface->m_Registry.view<RelationshipComponent, TransformComponent>();
			//using TraverseFun = std::function<void(entt::entity)>;
			//TraverseFun traverse = [&](entt::entity entity) {
			//	auto& relationship = view.get<RelationshipComponent>(entity);
			//	for (auto child : relationship.Children)
			//	{
			//		auto& childTransform = view.get<TransformComponent>(child);
			//		//childTransform.DeltaPosition = transform.DeltaPosition;
			//		childTransform.DeltaPosition = deltaPosition;
			//	}
			//	for (auto child : relationship.Children)
			//	{
			//		traverse(child);
			//	}
			//};
			//traverse(m_SelectedSceneEntity);
		}
	}

	void Editor::ShowFileDialogBox(const std::string& option, std::string& fullPath)
	{
		OPENFILENAMEA ofn;      // common dialog box structure
		char filePath[260];     // buffer for file path
		HWND hwnd;              // owner window
		HANDLE hf;              // file handle

		// Initialize OPENFILENAMEA
		memset(&ofn, 0, sizeof(ofn));
		memset(&hwnd, 0, sizeof(hwnd));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = filePath;
		/*
		Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		use the contents of szFile to initialize itself.
		*/ 
		ofn.lpstrFile[0] = '\0';
		if (option == "Save As")
		{
			ofn.nMaxFile = sizeof(filePath);
			ofn.lpstrFilter = "*.json\0";
		}
		ofn.nMaxFile = sizeof(filePath);
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (option == "Save As")
		{
			// Display the Save As dialog box. 
			if (GetSaveFileNameA(&ofn))
			{
				std::string strFilePath(ofn.lpstrFile);
				std::string strFileExtension(ofn.lpstrFilter);
				//std::string strFileName = FileSystem::FilePath(strFilePath + strFileExtension.substr(1)).filename().string();

				fullPath = strFilePath + strFileExtension.substr(1);
			}
		}

		if (option == "Open")
		{
			// Display the Open file dialog box. 
			if (GetOpenFileNameA(&ofn))
			{
				std::string strFilePath(ofn.lpstrFile);
				//std::string strFileName = FileSystem::FilePath(strFilePath).filename().string();

				fullPath = strFilePath;
			}
		}
	}

	void Editor::ShowFolderMenuPopup(const std::string& folderPath)
	{
		static char newFolderName[32] = "";
		std::string menuAction = "";
		if (ImGui::BeginPopup(folderPath.c_str()))
		{
			if (ImGui::MenuItem("New")) { menuAction = "New"; }
			if (ImGui::MenuItem("Delete")) { menuAction = "Delete"; }
			if (ImGui::MenuItem("Cancel")) { menuAction = "Cancel"; }
			ImGui::EndPopup();
		}

		ImGui::PushID(folderPath.c_str());
		if (menuAction == "New") { ImGui::OpenPopup("New"); }
		if (menuAction == "Delete") { ImGui::OpenPopup("Delete"); }
		if (menuAction == "Cancel") {}

		if (ImGui::BeginPopup("New"))
		{
			ImGui::Text("Enter new folder name:");
			ImGui::InputText("##folderName", newFolderName, IM_ARRAYSIZE(newFolderName));
			if (ImGui::Button("OK"))
			{
				FileSystem::CreateNewFolder(folderPath + "\\" + newFolderName);
				memset(newFolderName, 0, 32 * sizeof(newFolderName[0]));
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopup("Delete"))
		{
			ImGui::Text("Are you sure you want to delete this folder?");
			ImGui::MenuItem("***before deleting click arrow to close folder***", NULL, false, false);
			if (ImGui::Button("OK"))
			{
				FileSystem::Delete(folderPath);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();
	}

	void Editor::ShowHierarchyMenuPopup(entt::entity node, entt::registry& registry)
	{
		std::string stringNodeId;
		bool isSceneRootNode = false;
		if (node == entt::null)
		{
			isSceneRootNode = true;
			stringNodeId = std::to_string(1);
		}
		else
		{
			stringNodeId = std::to_string((uint32_t)node + 2);
		}
		
		static char newUiWidgetName[32] = "";
		static char newSpriteName[32] = "";
		std::string menuAction = "";

		if (ImGui::BeginPopup(stringNodeId.c_str()))
		{
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Canvas")) CreateWidget("Canvas", node);
				if (ImGui::MenuItem("Button")) CreateWidget("Button", node);
				if (ImGui::MenuItem("Text"))   CreateWidget("Text", node);

				ImGui::EndMenu();
			}
			if (!isSceneRootNode && ImGui::MenuItem("Rename")) { menuAction = "Rename"; }
			if (!isSceneRootNode && ImGui::MenuItem("Copy")) { menuAction = "Copy"; }
			if (!isSceneRootNode && ImGui::MenuItem("Delete")) { menuAction = "Delete"; }
			if (ImGui::MenuItem("Cancel")) { menuAction = "Cancel"; }
			ImGui::EndPopup();
		}

		ImGui::PushID(stringNodeId.c_str());
		if (menuAction == "Sprite") { ImGui::OpenPopup("Sprite"); }
		if (menuAction == "Rename") { ImGui::OpenPopup("Rename"); }
		if (menuAction == "Copy")	{ CreateCopyEntity(node); }
		if (menuAction == "Delete") { ImGui::OpenPopup("Delete"); }
		if (menuAction == "Cancel") {}

		if (ImGui::BeginPopup("Sprite"))
		{
			ImGui::Text("Enter sprite's name:");
			ImGui::InputText("##spriteName", newSpriteName, IM_ARRAYSIZE(newSpriteName));
			if (ImGui::Button("OK"))
			{
				auto newEntity = m_Scene->CreateEntity();
				//auto newEntity = m_UserInterface->CreateWidget();
				
				auto& tag = newEntity.AddComponent<EntityTagComponent>();
				auto& transform = newEntity.AddComponent<TransformComponent>();
				auto& relationship = newEntity.AddComponent<RelationshipComponent>();
				auto& sprite = newEntity.AddComponent<SpriteComponent>();
				
				transform.Position = glm::vec3(0.0f, 0.0f, 0.0f);
				transform.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
				transform.Scale = glm::vec3(100.0f, 100.0f, 1.0f);
				tag.Name = newSpriteName;
				relationship.Parent = node;
				SpriteRenderer::Submit(sprite);

				if (node != entt::null)
				{
					//auto& parentRelationship = m_Scene->m_Registry.get<RelationshipComponent>(node);
					auto& parentRelationship = registry.get<RelationshipComponent>(node);
					parentRelationship.Children.push_back(newEntity.GetEnttEntity());
					parentRelationship.NumOfChildren++;
				}

				memset(newSpriteName, 0, 32 * sizeof(newSpriteName[0]));
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("Rename"))
		{
			ImGui::Text("Enter new name:");
			ImGui::InputText("##name", newSpriteName, IM_ARRAYSIZE(newSpriteName));
			if (ImGui::Button("OK"))
			{
				//auto& tag = m_Scene->m_Registry.get<EntityTagComponent>(node);
				auto& tag = registry.get<EntityTagComponent>(node);
				tag.Name = newSpriteName;

				memset(newSpriteName, 0, 32 * sizeof(newSpriteName[0]));
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("Delete"))
		{
			ImGui::Text("Are you sure you want to delete this game object?");
			ImGui::MenuItem("", NULL, false, false);

			using TraverseFun = std::function<void(entt::entity)>;

			if (ImGui::Button("OK"))
			{
				// If entity is selected, then unselect it in order to delete it (this way there won't be any bugs)
				if (m_SelectedSceneEntity != entt::null)
					m_SelectedSceneEntity = entt::null;

				TraverseFun Traverse = [&](entt::entity entity) {
					//auto relationship = m_Scene->m_Registry.get<RelationshipComponent>(entity);
					auto relationship = registry.get<RelationshipComponent>(entity);
					for (auto child : relationship.Children)
						Traverse(child);

					//m_Scene->m_Registry.destroy(entity);
					registry.destroy(entity);

					if (relationship.Parent != entt::null)
					{
						//auto& parentRelationship = m_Scene->m_Registry.get<RelationshipComponent>(relationship.Parent);
						auto& parentRelationship = registry.get<RelationshipComponent>(relationship.Parent);
						uint32_t index = 0;
						for (auto child : parentRelationship.Children)
						{
							if (child == node)
							{
								parentRelationship.Children.erase(parentRelationship.Children.begin() + index);
								parentRelationship.NumOfChildren--;
								break;
							}
							index++;
						}
					}
				};

				Traverse(node);

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();
	}

	void Editor::CreateWidget(const std::string& name, entt::entity parent)
	{
		auto newWidget = m_Scene->CreateEntity();
		//auto newWidget = m_UserInterface->CreateWidget();

		auto& tag = newWidget.AddComponent<EntityTagComponent>();
		auto& transform = newWidget.AddComponent<TransformComponent>();
		auto& relationship = newWidget.AddComponent<RelationshipComponent>();
		auto& sprite = newWidget.AddComponent<SpriteComponent>();
		if (name == "Canvas")
			//newWidget.AddComponent<CanvasWidgetComponent>();
		if (name == "Button")
			//newWidget.AddComponent<ButtonWidgetComponent>();
		if (name == "Text")
			//newWidget.AddComponent<TextWidgetComponent>();

		tag.Name = name;
		relationship.Parent = parent;
		SpriteRenderer::Submit(sprite);

		if (parent != entt::null)
		{
			auto& parentTransform = m_Scene->m_Registry.get<TransformComponent>(parent);
			//auto& parentTransform = m_UserInterface->m_Registry.get<TransformComponent>(parent);
			transform.Position = parentTransform.Position;
			transform.Rotation = parentTransform.Rotation;
			if (name == "Text")
				transform.Position.x += parentTransform.Scale.x / 2.0f; // Pass center position so text can be rendered at centered position compared to parent widget

			auto& parentRelationship = m_Scene->m_Registry.get<RelationshipComponent>(parent);
			//auto& parentRelationship = m_UserInterface->m_Registry.get<RelationshipComponent>(parent);
			parentRelationship.Children.push_back(newWidget.GetEnttEntity());
			parentRelationship.NumOfChildren++;
		}
		else
		{
			transform.Position = glm::vec3(0.0f, 0.0f, 0.0f);
			transform.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		if (name == "Text")
			transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		else
			transform.Scale = glm::vec3(100.0f, 100.0f, 1.0f);
	}

	void Editor::CreateCanvasMatrixWidget(int dimensions[2], float padding[4], float cellWidgetSize[2], entt::entity parent)
	{
		int rows    = dimensions[0];
		int columns = dimensions[1];

		float paddingLeft   = padding[0];
		float paddingRight  = padding[1];
		float paddingTop    = padding[2];
		float paddingBottom = padding[3];

		float cellWidgetWidth  = cellWidgetSize[0];
		float cellWidgetHeight = cellWidgetSize[1];

		float widgetWidth = paddingLeft + paddingRight + (columns - 1) * paddingLeft + columns * cellWidgetWidth;
		float widgetHeight = paddingTop + paddingBottom + (rows - 1) * paddingTop + rows * cellWidgetHeight;

		for (int row = 0; row < rows; ++row)
		{
			for (int column = 0; column < columns; ++column)
			{
				// Cell canvas widget is a slot (placeholder) for widgets such as buttons
				auto canvas = m_Scene->CreateEntity();

				//auto& cellWidgetCanvas = canvas.AddComponent<CanvasWidgetComponent>();
				auto& cellWidgetRelationship = canvas.AddComponent<RelationshipComponent>();
				auto& cellWidgetSprite = canvas.AddComponent<SpriteComponent>();
				auto& cellWidgetTransform = canvas.AddComponent<TransformComponent>();
				auto& entityTagComponent = canvas.AddComponent<EntityTagComponent>();

				entityTagComponent.Name = "Canvas";
				cellWidgetSprite.Color = glm::vec4(0.8f, 0.8f, 0.8f, 0.7f);
				SpriteRenderer::Submit(cellWidgetSprite);

				auto& transform = m_Scene->m_Registry.get<TransformComponent>(parent);
				if (column == 0)
					cellWidgetTransform.Position.x = transform.Position.x + paddingLeft;
				else
					cellWidgetTransform.Position.x = transform.Position.x + (column + 1) * paddingLeft + column * cellWidgetWidth;
				if (row == 0)
					cellWidgetTransform.Position.y = transform.Position.y - paddingTop;
				else
					cellWidgetTransform.Position.y = transform.Position.y - (row + 1) * paddingTop - row * cellWidgetHeight;
				cellWidgetTransform.Scale.x = cellWidgetWidth;
				cellWidgetTransform.Scale.y = cellWidgetHeight;

				//cellWidgetRelationship.Parent = m_SelectedSceneEntity;
				cellWidgetRelationship.Parent = parent;
				auto& relationship = m_Scene->m_Registry.get<RelationshipComponent>(parent);
				relationship.Children.push_back(canvas.GetEnttEntity());
				relationship.NumOfChildren++;
			}
		}

		auto& transform = m_Scene->m_Registry.get<TransformComponent>(parent);
		transform.Scale.x = widgetWidth;
		transform.Scale.y = widgetHeight;
	
		ImGui::NewLine();
	}

	void Editor::CreateCopyEntity(entt::entity node) // Entity can have many children or none
	{
		using TraverseFun = std::function<void(entt::entity, entt::entity)>;

		TraverseFun traverse = [&](entt::entity node, entt::entity copyNode) {

			auto nodeRelationship = m_Scene->m_Registry.get<RelationshipComponent>(node); // Get by value because entt library change this component under the hood when new component is added to registry (TODO: Check this weird feature!)

			for (auto child : nodeRelationship.Children)
			{
				auto copyEntity = m_Scene->m_Registry.create();

				if (m_Scene->m_Registry.has<EntityTagComponent>(child))
					m_Scene->m_Registry.emplace<EntityTagComponent>(copyEntity, m_Scene->m_Registry.get<EntityTagComponent>(child));

				if (m_Scene->m_Registry.has<TransformComponent>(child))
					m_Scene->m_Registry.emplace<TransformComponent>(copyEntity, m_Scene->m_Registry.get<TransformComponent>(child));

				if (m_Scene->m_Registry.has<MeshComponent>(child))
					m_Scene->m_Registry.emplace<MeshComponent>(copyEntity, m_Scene->m_Registry.get<MeshComponent>(child));

				if (m_Scene->m_Registry.has<BoundBoxComponent>(child))
					m_Scene->m_Registry.emplace<BoundBoxComponent>(copyEntity, m_Scene->m_Registry.get<BoundBoxComponent>(child));

				if (m_Scene->m_Registry.has<SpriteComponent>(child))
					m_Scene->m_Registry.emplace<SpriteComponent>(copyEntity, m_Scene->m_Registry.get<SpriteComponent>(child));

				//if (m_Scene->m_Registry.has<CanvasWidgetComponent>(child))
				//	m_Scene->m_Registry.emplace<CanvasWidgetComponent>(copyEntity, m_Scene->m_Registry.get<CanvasWidgetComponent>(child));

				//if (m_Scene->m_Registry.has<ButtonWidgetComponent>(child))
				//	m_Scene->m_Registry.emplace<ButtonWidgetComponent>(copyEntity, m_Scene->m_Registry.get<ButtonWidgetComponent>(child));

				//if (m_Scene->m_Registry.has<TextWidgetComponent>(child))
				//	m_Scene->m_Registry.emplace<TextWidgetComponent>(copyEntity, m_Scene->m_Registry.get<TextWidgetComponent>(child));

				auto& copyRelationship = m_Scene->m_Registry.emplace<RelationshipComponent>(copyEntity);
				copyRelationship.Parent = copyNode; // copyNode is the parent of copyEntity
				auto& copyParentRelationship = m_Scene->m_Registry.get<RelationshipComponent>(copyNode);
				copyParentRelationship.Children.push_back(copyEntity);
				copyParentRelationship.NumOfChildren++;
			}

			int index = 0;
			auto& copyNodeRelationship = m_Scene->m_Registry.get<RelationshipComponent>(copyNode);
			for (auto child : nodeRelationship.Children)
			{
				auto copyChild = copyNodeRelationship.Children[index];
				traverse(child, copyChild);
				index++;
			}
		};

		// Create copy root (starting) node
		auto copyNode = m_Scene->m_Registry.create();
		auto& relationship = m_Scene->m_Registry.get<RelationshipComponent>(node);
		if (relationship.Parent != entt::null) // If node's parent isn't scene root node
		{
			auto& parentRelationship = m_Scene->m_Registry.get<RelationshipComponent>(relationship.Parent);
			parentRelationship.Children.push_back(copyNode); // Push copy node to parent's children
			parentRelationship.NumOfChildren++;
		}
		if (m_Scene->m_Registry.has<EntityTagComponent>(node))
			m_Scene->m_Registry.emplace<EntityTagComponent>(copyNode, m_Scene->m_Registry.get<EntityTagComponent>(node));

		if (m_Scene->m_Registry.has<TransformComponent>(node))
			m_Scene->m_Registry.emplace<TransformComponent>(copyNode, m_Scene->m_Registry.get<TransformComponent>(node));

		if (m_Scene->m_Registry.has<MeshComponent>(node))
			m_Scene->m_Registry.emplace<MeshComponent>(copyNode, m_Scene->m_Registry.get<MeshComponent>(node));

		if (m_Scene->m_Registry.has<BoundBoxComponent>(node))
			m_Scene->m_Registry.emplace<BoundBoxComponent>(copyNode, m_Scene->m_Registry.get<BoundBoxComponent>(node));

		if (m_Scene->m_Registry.has<SpriteComponent>(node))
			m_Scene->m_Registry.emplace<SpriteComponent>(copyNode, m_Scene->m_Registry.get<SpriteComponent>(node));

		//if (m_Scene->m_Registry.has<CanvasWidgetComponent>(node))
		//	m_Scene->m_Registry.emplace<CanvasWidgetComponent>(copyNode, m_Scene->m_Registry.get<CanvasWidgetComponent>(node));

		//if (m_Scene->m_Registry.has<ButtonWidgetComponent>(node))
		//	m_Scene->m_Registry.emplace<ButtonWidgetComponent>(copyNode, m_Scene->m_Registry.get<ButtonWidgetComponent>(node));

		//if (m_Scene->m_Registry.has<TextWidgetComponent>(node))
		//	m_Scene->m_Registry.emplace<TextWidgetComponent>(copyNode, m_Scene->m_Registry.get<TextWidgetComponent>(node));

		//auto& copyRelationship = m_Scene->m_Registry.emplace<RelationshipComponent>(copyNode, m_Scene->m_Registry.get<RelationshipComponent>(node));
		auto& copyRelationship = m_Scene->m_Registry.emplace<RelationshipComponent>(copyNode);
		copyRelationship.Parent = relationship.Parent;
		traverse(node, copyNode);
	}

	/**
	 * @brief Create second window for "play" mode.
	 *
	 * This method creates second window, when user clicks "Play" button.
	 * This window will render the final look of a game, together with in-game UI.
	 */
	void Editor::CreateGameWindow()
	{
		Window::Hint(GLFW_DECORATED, true);
		m_GameWindow->CreateGLFWwindow(NULL, m_MainWindow->m_GLFWwindow);

		Window::MakeContextCurrent(m_GameWindow->m_GLFWwindow);

		glfwSetWindowAspectRatio(m_GameWindow->m_GLFWwindow, m_GameWindow->m_Width, m_GameWindow->m_Height);

		m_GameWindow->SetEventCallbacks();
		m_MainWindow->RemoveEventCallbacks();

		UserInput::SetGLFWwindow(m_GameWindow->m_GLFWwindow);

		m_GameWindow->ShowWindow();

		auto& fromRegistry = m_Scene->m_Registry;
		auto& toRegistry = m_GameWindow->m_Scene->m_Registry;
		toRegistry.clear();

		m_Scene->m_CloneFunctions[entt::type_hash<TransformComponent>::value()] = &Scene::CloneRegistry<TransformComponent>;
		m_Scene->m_CloneFunctions[entt::type_hash<RelationshipComponent>::value()] = &Scene::CloneRegistry<RelationshipComponent>;
		m_Scene->m_CloneFunctions[entt::type_hash<EntityTagComponent>::value()] = &Scene::CloneRegistry<EntityTagComponent>;
		m_Scene->m_CloneFunctions[entt::type_hash<BoundBoxComponent>::value()] = &Scene::CloneRegistry<BoundBoxComponent>;
		m_Scene->m_CloneFunctions[entt::type_hash<MeshComponent>::value()] = &Scene::CloneRegistry<MeshComponent>;
		m_Scene->m_CloneFunctions[entt::type_hash<SpriteComponent>::value()] = &Scene::CloneRegistry<SpriteComponent>;
		m_Scene->m_CloneFunctions[entt::type_hash<AnimationComponent>::value()] = &Scene::CloneRegistry<AnimationComponent>;
		m_Scene->m_CloneFunctions[entt::type_hash<CameraComponent>::value()] = &Scene::CloneRegistry<CameraComponent>;
		m_Scene->m_CloneFunctions[entt::type_hash<ScriptComponent>::value()] = &Scene::CloneRegistry<ScriptComponent>;
		//m_Scene->m_CloneFunctions[entt::type_hash<TextWidgetComponent>::value()] = &Scene::CloneRegistry<TextWidgetComponent>;
		//m_Scene->m_CloneFunctions[entt::type_hash<CanvasWidgetComponent>::value()] = &Scene::CloneRegistry<CanvasWidgetComponent>;
		//m_Scene->m_CloneFunctions[entt::type_hash<ButtonWidgetComponent>::value()] = &Scene::CloneRegistry<ButtonWidgetComponent>;

		// Create entities with same id as entities in m_Scene
		fromRegistry.each([&toRegistry](auto entity)
			{
				auto copyEntity = toRegistry.create(entity);
			}
		);

		// Iterate over all components of m_Editor->m_Scene and copy components to m_GameWindow->m_Scene
		m_Scene->m_Registry.visit([this, &fromRegistry, &toRegistry](const auto componentType)
			{
				m_Scene->m_CloneFunctions[componentType.hash()](fromRegistry, toRegistry);
			}
		);

		auto view = m_GameWindow->m_Scene->GetView<MeshComponent>();
		for (auto entity : view)
		{
			auto& mesh = view.get<MeshComponent>(entity);
			MeshRenderer::SubmitCopy(mesh);
		}

		Window::MakeContextCurrent(m_MainWindow->m_GLFWwindow);
	}

	/**
	 * @brief Create a frame buffer.
	 *
	 * This method creates frame buffer, which will be used for Scene window in editor.
	 */
	void Editor::CreateFrameBuffer(int width, int height)
	{
		m_FrameBuffer = sPtrCreate<FrameBuffer>();
		m_IntermediateFrameBuffer = sPtrCreate<FrameBuffer>();
		m_RenderBuffer = sPtrCreate<RenderBuffer>();
		//m_Texture = sPtrCreate<Texture>(width, height);
		m_Texture = sPtrCreate<Texture>(width, height, 4);
		m_IntermediateTexture = sPtrCreate<Texture>(width, height);

		m_FrameBuffer->Bind();
		m_RenderBuffer->Bind();
		//m_RenderBuffer->Storage(width, height);
		m_RenderBuffer->Storage(width, height, 4);
		m_RenderBuffer->Unbind();
		m_FrameBuffer->AttachMultisampled(m_Texture->GetId(), FrameBuffer::AttachmentType::TextureBuffer);
		m_FrameBuffer->AttachMultisampled(m_RenderBuffer->GetId(), FrameBuffer::AttachmentType::RenderBuffer);
		//m_FrameBuffer->Attach(m_Texture->GetId(), FrameBuffer::AttachmentType::TextureBuffer);
		//m_FrameBuffer->Attach(m_RenderBuffer->GetId(), FrameBuffer::AttachmentType::RenderBuffer);
		m_FrameBuffer->Unbind();

		m_IntermediateFrameBuffer->Bind();
		m_IntermediateFrameBuffer->Attach(m_IntermediateTexture->GetId(), FrameBuffer::AttachmentType::TextureBuffer);
		m_IntermediateFrameBuffer->Unbind();
	}

	/**
	 * @brief Bind frame buffer.
	 *
	 * Frame buffer is bound before Scene rendering.
	 */
	void Editor::BindFrameBuffer()
	{
		m_FrameBuffer->Bind();
	}

	/**
	 * @brief Unbind frame buffer.
	 *
	 * Frame buffer is unbound after Scene rendering is done.
	 */
	void Editor::UnbindFrameBuffer()
	{
		m_FrameBuffer->Unbind();
	}

	/**
	 * @brief Import asset in Scene window
	 *
	 * Asset is imported on drag'n'drop.
	 * Asset could be found in Project windo, in editor.
	 */
	void Editor::ImportAsset(const std::string& importAssetFilePath)
	{
		auto& [loadedMesh, loadedAnimation] = ModelImporter::LoadMeshFile(importAssetFilePath);

		auto meshEntity = m_Scene->CreateEntity();

		auto& entityTag = meshEntity.AddComponent<EntityTagComponent>(importAssetFilePath);
		auto& mesh = meshEntity.AddComponent<MeshComponent>(*loadedMesh);
		auto& transform = meshEntity.AddComponent<TransformComponent>();
		auto& relationship = meshEntity.AddComponent<RelationshipComponent>();

		relationship.Parent = entt::null;

		meshEntity.AddComponent<BoundBoxComponent>(*loadedMesh);
		if (loadedMesh->HasAnimation)
			auto& animation = meshEntity.AddComponent<AnimationComponent>(*loadedAnimation);

		// Check if mouse ray intersects ground plane
		if (MouseRay::CheckCollisionWithPlane())
		{
			// Place asset on the intersection point
			const glm::vec3& intersectionPoint = MouseRay::GetIntersectionPoint();
			transform.Position = intersectionPoint;
		}
		else
		{
			// Place asset on the origin point of world coordinate system
			transform.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		transform.Scale = glm::vec3(10.0f, 10.0f, 10.0f);

		MeshRenderer::Submit(mesh);
	}

	/**
	 * @brief Select mesh on left click
	 *
	 * Mesh is selected when user clicks on it in Scene window.
	 * Editor will keep track of the selected entity ID, which is of the type entt::entity.
	 */
	void Editor::SelectMesh()
	{
		auto& camera = m_Scene->GetCamera();
		MouseRay::CalculateRayOrigin(camera, m_MouseInfo.MousePosX, m_MouseInfo.MousePosY, m_SceneInfo.SceneWidth, m_SceneInfo.SceneHeight);

		auto view = m_Scene->m_Registry.view<MeshComponent, TransformComponent, BoundBoxComponent>();

		bool isAnyMeshSelected = false;
		std::pair<entt::entity, int> minIntersectionDistance = std::make_pair(entt::null, -1); // If ray goes through many meshes, then take minimum intersection distance

		for (auto entity : view)
		{
			const auto& [mesh, transform] = view.get<MeshComponent, TransformComponent>(entity);

			// Check if mouse ray intersects AABB of a mesh
			if (MouseRay::CheckCollisionWithBox(mesh, transform))
			{
				// Check if mouse ray intersects any mesh triangle
				if (MouseRay::CheckCollisionWithMesh(mesh, transform))
				{
					isAnyMeshSelected = true;

					if (minIntersectionDistance.first == entt::null)
					{
						minIntersectionDistance.first = entity;
						minIntersectionDistance.second = (int)MouseRay::GetCollisionDistance();
					}
					else
					{
						if (minIntersectionDistance.second > (int)MouseRay::GetCollisionDistance())
						{
							minIntersectionDistance.first = entity;
							minIntersectionDistance.second = (int)MouseRay::GetCollisionDistance();
						}
					}
				}
			}
		}

		if (isAnyMeshSelected)
		{
			m_SelectedSceneEntity = minIntersectionDistance.first;
			s_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		}
		else
		{
			m_SelectedSceneEntity = entt::null;
			s_CurrentGizmoOperation = ImGuizmo::BOUNDS;
		}
	}

	/**
	 * @brief Update Scene texture when Scene window is resized.
	 *
	 * This method resizes framebuffer according to Scene window size and it also updates viewport (glViewport function).
	 */
	void Editor::UpdateSceneViewport(float sceneWindowWidth, float sceneWindowHeight)
	{
		float ratio = 1.5f;
		
		float x = sceneWindowWidth;
		float y = sceneWindowHeight;

		if ((x / y) > ratio)
		{
			x = ratio * y;
		}
		if ((x / y) < ratio)
		{
			y = (1 / ratio) * x;
		}

		//m_Texture->Resize((int)x, (int)y);
		m_Texture->ResizeMultisampled((int)x, (int)y, 4);
		m_IntermediateTexture->Resize((int)x, (int)y);
		m_RenderBuffer->ResizeMultisampled((int)x, (int)y, 4);
		//m_RenderBuffer->Resize((int)x, (int)y);

		glViewport(0, 0, (int)x, (int)y);

		m_SceneInfo.SceneWidth = (int)x;
		m_SceneInfo.SceneHeight = (int)y;
	}

}