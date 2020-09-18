#include "pch.h"
#include "UserInterface.h"
#include "Core/FileSystem.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Renderer/SpriteRenderer.h"

namespace Yugo
{

	UserInterface::UserInterface(Scene* scene)
		: m_Scene(scene)
	{
		//Dispatcher::Subscribe<MouseButtonPress>(this);
		//Dispatcher::Subscribe<KeyboardKeyPress>(this);
		//Dispatcher::Subscribe<MouseScroll>(this);
	}

	void UserInterface::OnStart()
	{
		Shader uiShader(
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\ui_vertex.shader",
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Shaders\\ui_fragment.shader"
		);
		ResourceManager::AddShader("uiShader", uiShader);

		Texture texture(
			FileSystem::GetSolutionFolderPath() + "Main\\src\\Assets\\Textures\\awesomeface.png"
		);
		ResourceManager::AddTexture("texture", texture);

		// Button example
		auto sprite = m_Scene->CreateEntity();
		auto& spriteComponent = sprite.AddComponent<SpriteComponent>();
		auto& transformComponent = sprite.AddComponent<TransformComponent>();
		auto& entityTagComponent = sprite.AddComponent<EntityTagComponent>();

		transformComponent.Position = glm::vec3(400.0f, -400.0f, 0.0f);
		transformComponent.Scale = glm::vec3(50.0f, 50.0f, 0.0f);
		transformComponent.ModelMatrix = glm::mat4(1.0f);
		transformComponent.ModelMatrix = glm::translate(transformComponent.ModelMatrix, transformComponent.Position);
		transformComponent.ModelMatrix = glm::scale(transformComponent.ModelMatrix, transformComponent.Scale);

		spriteComponent.Texture = ResourceManager::GetTexture("texture");

		entityTagComponent.Name = "Button";

		SpriteRenderer::Submit(spriteComponent);
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
					if (MouseRay::CheckCollisionWithSprite(sprite, transform))
						std::cout << "da!\n";
					else
						std::cout << "ne!\n";
				}
			}
		}
	}

	void UserInterface::OnUpdate(float ts)
	{

	}

	void UserInterface::OnRender()
	{
		auto view = m_Scene->m_Registry.view<SpriteComponent, TransformComponent>();
		for (auto entity : view)
		{
			auto& [sprite, transform] = view.get<SpriteComponent, TransformComponent>(entity);
			SpriteRenderer::Render(sprite, transform, ResourceManager::GetShader("uiShader"));
		}
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

}