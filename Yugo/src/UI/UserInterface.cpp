#include "pch.h"
#include "UserInterface.h"
#include "Core/FileSystem.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Renderer/SpriteRenderer.h"
#include "Renderer/TextRenderer.h"
#include "Widget.h"


namespace Yugo
{
	using TraverseUpdateFun = std::function<void(entt::entity, TransformComponent&)>;
	using TraverseRenderFun = std::function<void(entt::entity)>;

	UserInterface::UserInterface(sPtr<Scene>& scene)
		: m_Scene(scene)
	{
		m_Camera = sPtrCreate<Camera>(glm::vec3(100.0f, 100.0f, 100.0f));

		//Dispatcher::Subscribe<MouseButtonPress>(this);
		//Dispatcher::Subscribe<KeyboardKeyPress>(this);
		//Dispatcher::Subscribe<MouseScroll>(this);
	}

	void UserInterface::OnStart()
	{
		Shader uiTexturedShader(
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\ui_vertex.shader",
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\ui_textured_fragment.shader"
		);
		ResourceManager::AddShader("uiTexturedShader", uiTexturedShader);

		Shader uiFlatColoredShader(
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\ui_vertex.shader",
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\ui_flat_colored_fragment.shader"
		);
		ResourceManager::AddShader("uiFlatColoredShader", uiFlatColoredShader);

		Texture texture(
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Textures\\awesomeface.png"
		);
		ResourceManager::AddTexture("texture", texture);

		SpriteRenderer::SetCamera(m_Camera);
		TextRenderer::SetCamera(m_Camera);
		TextRenderer::Submit();
	}

	void UserInterface::OnEvent(const Event& event)
	{
		if (event.GetEventType() == EventType::MouseButtonPress)
		{
			const auto& mouseButtonPress = static_cast<const MouseButtonPress&>(event);
			if (mouseButtonPress.GetButtonCode() == MOUSE_BUTTON_LEFT)
			{
				auto view = m_Scene->m_Registry.view<SpriteComponent, TransformComponent>();
				for (auto entity : view)
				{
					const auto& [sprite, transform] = view.get<SpriteComponent, TransformComponent>(entity);
					if (MouseRay::CheckCollisionWithSprite(transform))
						std::cout << "da!\n";
					else
						std::cout << "ne!\n";
				}
			}
		}
	}

	void UserInterface::OnUpdate(float ts)
	{
		// In-order traversal (recursive method)
		TraverseUpdateFun traverse = [&](entt::entity entity, TransformComponent& transform) {
			auto view = m_Scene->m_Registry.view<SpriteComponent, TransformComponent, RelationshipComponent>();
			auto& [relationship, nodeTransform] = view.get<RelationshipComponent, TransformComponent>(entity);
			if (relationship.Parent == entt::null) // Update all parent widgets with scene root node as their parent
			{
				transform.ModelMatrix = glm::mat4(1.0f);
				transform.ModelMatrix = glm::translate(transform.ModelMatrix, transform.Position);
				transform.ModelMatrix = glm::scale(transform.ModelMatrix, transform.Scale);
			}
			else // Update child widgets position with delta (the amount by which parent widget has been moved)
			{
				nodeTransform.Position += transform.DeltaPosition;
				nodeTransform.ModelMatrix = glm::mat4(1.0f);
				nodeTransform.ModelMatrix = glm::translate(nodeTransform.ModelMatrix, nodeTransform.Position);
				nodeTransform.ModelMatrix = glm::scale(nodeTransform.ModelMatrix, nodeTransform.Scale);
			}

			for (auto child : relationship.Children)
			{
				traverse(child, transform);
			}
		};

		auto view = m_Scene->m_Registry.view<SpriteComponent, TransformComponent, RelationshipComponent>();

		for (auto entity : view)
		{
			auto& [relationship, transform] = view.get<RelationshipComponent, TransformComponent>(entity);
			if (relationship.Parent == entt::null) // Update only root widgets, which parent is entt::null (scene root node)
				traverse(entity, transform);
		}
	}

	void UserInterface::OnRender()
	{
		// Level-order traversal (iterative method)
		TraverseRenderFun traverse = [&](entt::entity entity) {
			std::queue<entt::entity> queue;
			queue.push(entity);
			
			auto view = m_Scene->m_Registry.view<SpriteComponent, TransformComponent, RelationshipComponent>();

			while (!queue.empty())
			{
				entity = queue.front();
				queue.pop();
				auto& [sprite, transform, relationship] = view.get<SpriteComponent, TransformComponent, RelationshipComponent>(entity);
				
				if (m_Scene->m_Registry.has<TextWidgetComponent>(entity))
				{
					auto& text = m_Scene->m_Registry.get<TextWidgetComponent>(entity);
					TextRenderer::Render(text, transform, ResourceManager::GetShader("textShader"));
				}
				else
				{
					if (sprite.HasTexture)
						SpriteRenderer::Render(sprite, transform, ResourceManager::GetShader("uiTexturedShader"));
					else
						SpriteRenderer::Render(sprite, transform, ResourceManager::GetShader("uiFlatColoredShader"));
				}

				for (auto child : relationship.Children)
				{
					queue.push(child);
				}
			}
		};

		auto view = m_Scene->m_Registry.view<SpriteComponent, TransformComponent, RelationshipComponent>();
		for (auto entity : view)
			traverse(entity);
	}

	void UserInterface::OnShutdown()
	{
	}

	void UserInterface::SaveUserInerface()
	{
	}

	void UserInterface::LoadUserInterface()
	{
	}

	Widget UserInterface::CreateWidget(const std::string& name)
	{
		Widget widget = { m_Registry.create(), name, this };
		return widget;
	}

}