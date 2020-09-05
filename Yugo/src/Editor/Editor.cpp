#include "pch.h"
#include "Editor.h"
#include "Animation/AnimationComponent.h"
#include "Animation/Animation.h"
#include "Scripting/ScriptComponent.h"
#include "Core/Time.h"

#include <ImGuizmo.h>
#include <windows.h>
#include <commdlg.h>

namespace Yugo
{

	// Play mode flag
	static bool s_PlayMode = false;

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
	{
		m_SceneInfo.SceneName = "Default";
		m_SceneInfo.SceneFilePath = "None";
		m_SceneInfo.SceneWidth = 1200;
		m_SceneInfo.SceneHeight = 800;
		m_Scene = sPtrCreate<Scene>();
		m_ScriptEngine = uPtrCreate<ScriptEngine>();
	}

	void Editor::OnStart()
	{
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
		ImGui_ImplGlfw_InitForOpenGL(Window::GetGLFWwindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Initially create texture framebuffer with default size (1200x800)
		CreateFrameBuffer(m_SceneInfo.SceneWidth, m_SceneInfo.SceneHeight);

		m_Scene->OnStart();
		m_ScriptEngine->SetScene(m_Scene.get()); // TODO: Think about better solution!

		Dispatcher::Subscribe<MouseButtonPress>(this);
		Dispatcher::Subscribe<KeyboardKeyPress>(this);
		Dispatcher::Subscribe<ImportAssetEvent>(this);
		Dispatcher::Subscribe<MouseScroll>(this);
	}

	void Editor::OnRender()
	{
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

		if (!s_PlayMode)
		{
			ShowHierarchyWindow();
			ShowInspectorWindow();
			ShowProjectWindow();
		}
		ShowSceneWindow();
		

		ImGui::ShowDemoWindow();

		// Render Scene
		m_FrameBuffer->Bind();
		m_Scene->OnRender();
		m_FrameBuffer->Unbind();

		// Render ImGui
		ImGuiIO& io = ImGui::GetIO();
		int glfwWindowWidth;
		int glfwWindowHeight;
		glfwGetWindowSize(Window::GetGLFWwindow(), &glfwWindowWidth, &glfwWindowHeight);
		io.DisplaySize = ImVec2((float)glfwWindowWidth, (float)glfwWindowHeight);

		ImGui::Render();

		/*
		Clear default framebuffer color and depth buffers which are attchaed to it;
		This is needed because scene is being rendered on custom framebuffer's attached texture
		and ImGUI is being renderd on default framebuffer
		*/
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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

	void Editor::OnUpdate(float ts)
	{
		// The order of updates is important! If scene update goes first, then script can't execute entity movement
		//auto view = m_Scene->m_Registry.view<ScriptComponent>();
		if (s_PlayMode)
			m_ScriptEngine->OnUpdate(ts);

		m_Scene->OnUpdate(ts);

		// TEMPORARY!!!
		if (s_PlayMode)
		{
			auto view = m_Scene->m_Registry.view<TransformComponent>();
			for (auto entity : view)
			{
				auto& transform = view.get<TransformComponent>(entity);

				transform.ModelMatrix = glm::mat4(1.0f);
				transform.ModelMatrix = glm::translate(transform.ModelMatrix, transform.Position);
				transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				transform.ModelMatrix = glm::rotate(transform.ModelMatrix, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				transform.ModelMatrix = glm::scale(transform.ModelMatrix, transform.Scale);
			}
		}
	}

	void Editor::OnEvent(const Event& event)
	{
		if (event.GetEventType() == EventType::MouseButtonPress)
		{
			const auto& mouseButtonPress = static_cast<const MouseButtonPress&>(event);
			if (mouseButtonPress.GetButtonCode() == MOUSE_BUTTON_LEFT && m_IsSceneWindowHovered)
			{
				if (!ImGuizmo::IsOver() && !UserInput::IsKeyboardKeyPressed(KEY_LEFT_CONTROL))
				{
					m_Scene->CastMouseRay(m_MouseInfo.MousePosX, m_MouseInfo.MousePosY, m_SceneInfo.SceneWidth, m_SceneInfo.SceneHeight);
					m_Scene->OnEvent(event);
				}
			}
		}

		// Import asset event is invoked when user drag and drop asset to scene imgui window 
		if (event.GetEventType() == EventType::ImportAsset)
		{
			m_Scene->CastMouseRay(m_MouseInfo.MousePosX, m_MouseInfo.MousePosY, m_SceneInfo.SceneWidth, m_SceneInfo.SceneHeight);
			m_Scene->OnEvent(event);
		}

		if (event.GetEventType() == EventType::MouseScroll && m_IsSceneWindowHovered)
		{
			m_Scene->OnEvent(event);
		}
	}

	void Editor::OnShutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		m_ScriptEngine->OnShutdown();
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

	void Editor::ShowInspectorWindow()
	{
		ImGui::Begin("Inspector");

		if (m_Scene->IsAnyMeshSelected())
		{
			auto& transform = m_Scene->GetSelectedMeshTransform();

			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform.ModelMatrix), glm::value_ptr(transform.Position), glm::value_ptr(transform.Rotation), glm::value_ptr(transform.Scale));
			ImGui::InputFloat3("Translate", glm::value_ptr(transform.Position));
			ImGui::InputFloat3("Rotate", glm::value_ptr(transform.Rotation));
			ImGui::InputFloat3("Scale", glm::value_ptr(transform.Scale));
			ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(transform.Position), glm::value_ptr(transform.Rotation), glm::value_ptr(transform.Scale), glm::value_ptr(transform.ModelMatrix));
			
			ImGui::NewLine();

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

			auto view = m_Scene->m_Registry.view<MeshComponent, AnimationComponent>();
			for (auto entity : view)
			{
				auto& [mesh, animation] = view.get<MeshComponent, AnimationComponent>(entity);
				if (entity == m_Scene->m_SelectedEntity)
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

			static std::string scriptFilePath = "Script: None";
			ImGui::Text(scriptFilePath.c_str());

			if (ImGui::Button("Select Script"))
				ShowFileDialogBox("Open", scriptFilePath);
			
			ImGui::SameLine();

			if (m_Scene->m_Registry.has<ScriptComponent>(m_Scene->m_SelectedEntity))
			{
				if (ImGui::Button("Attach Script"))
				{
					auto& scriptComponent = m_Scene->m_Registry.get<ScriptComponent>(m_Scene->m_SelectedEntity);
					scriptComponent.ScriptFilePath = scriptFilePath;

					// TODO: Implement how to change Entity in ScriptBehaviour!
				}
			}
			else
			{
				if (ImGui::Button("Attach Script"))
				{
					auto& scriptComponent = m_Scene->m_Registry.emplace<ScriptComponent>(m_Scene->m_SelectedEntity);
					auto& entityTagComponent = m_Scene->m_Registry.get<EntityTagComponent>(m_Scene->m_SelectedEntity);

					scriptComponent.ScriptFilePath = scriptFilePath;
					Entity entity(m_Scene->m_SelectedEntity, entityTagComponent.Name, m_Scene.get());
					m_ScriptEngine->AttachScript(scriptFilePath, entity);
				}
			}
		}

		ImGui::End();
	}

	void Editor::ShowHierarchyWindow()
	{
		ImGui::Begin("Hierarchy", NULL);

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
			nodeClicked = rootId;

		if (rootNodeOpen)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()); // Increase spacing to differentiate leaves from expanded contents.
			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			auto view = m_Scene->m_Registry.view<EntityTagComponent, TransformComponent>();
			for (auto entity : view)
			{
				const auto& entityTag = view.get<EntityTagComponent>(entity);
				const auto& entityName = entityTag.Name;

				uint32_t entityId = rootId + (uint32_t)entity + 1;
				if (selection == entityId)
					nodeFlags |= ImGuiTreeNodeFlags_Selected;
				ImGui::TreeNodeEx((void*)(uintptr_t)entityId, nodeFlags, entityName.c_str());

				if (ImGui::IsItemClicked())
					nodeClicked = entityId;
			}

			if (nodeClicked != 0)
				selection = nodeClicked;

			ImGui::PopStyleVar();
			ImGui::TreePop();
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
		ImGui::Image((void*)m_Texture->GetId(), ImVec2((float)m_SceneInfo.SceneWidth, (float)m_SceneInfo.SceneHeight), ImVec2(0, 1), ImVec2(1, 0));
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

		if (m_Scene->IsAnyMeshSelected())
		{
			auto& transform = m_Scene->GetSelectedMeshTransform();
			const auto& projectionMatrix = m_Scene->m_Camera->GetProjectionMatrix();
			const auto& viewMatrix = m_Scene->m_Camera->GetViewMatrix();
			ShowImGuizmoWidget(transform, projectionMatrix, viewMatrix);
		}

		ImGui::End();

		ImGui::Begin("Scene Commands");
		
		if (ImGui::Button("Play Scene"))
		{
			s_PlayMode = true;
			m_ScriptEngine->OnStart();
		}

		ImGui::SameLine();
		
		if (ImGui::Button("Stop Scene"))
		{
			s_PlayMode = false;
			m_ScriptEngine->OnStop();
		}
		
		ImGui::End();
	}

	void Editor::ShowImGuizmoWidget(TransformComponent& transform, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
	{
		ImGuizmo::BeginFrame();
		ImGuizmo::SetOrthographic(true);

		if (ImGui::IsKeyPressed((int)KEY_T))
			s_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed((int)KEY_R))
			s_CurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed((int)KEY_Y))
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

	void Editor::CreateFrameBuffer(int width, int height)
	{
		m_FrameBuffer = sPtrCreate<FrameBuffer>();
		m_RenderBuffer = sPtrCreate<RenderBuffer>();
		m_Texture = sPtrCreate<Texture>(width, height);

		m_FrameBuffer->Bind();
		m_RenderBuffer->Bind();
		m_RenderBuffer->Storage(width, height);
		m_RenderBuffer->Unbind();
		m_FrameBuffer->Attach(m_Texture->GetId(), FrameBuffer::AttachmentType::TextureBuffer);
		m_FrameBuffer->Attach(m_RenderBuffer->GetId(), FrameBuffer::AttachmentType::RenderBuffer);
		m_FrameBuffer->Unbind();
	}

	void Editor::BindFrameBuffer()
	{
		m_FrameBuffer->Bind();
	}

	void Editor::UnbindFrameBuffer()
	{
		m_FrameBuffer->Unbind();
	}


	// This method resizes framebuffer according to ImGui scene window's size and it also updates viewport (glViewport function)
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

		m_Texture->Resize((int)x, (int)y);
		m_RenderBuffer->Resize((int)x, (int)y);

		glViewport(0, 0, (int)x, (int)y);

		m_SceneInfo.SceneWidth = (int)x;
		m_SceneInfo.SceneHeight = (int)y;
	}

}