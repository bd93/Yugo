#pragma once
#include "Events/Event.h"
#include "Events/KeyboardKeyEvent.h"
#include "Events/MouseButtonEvent.h"
#include "AllComponents.h"
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"
#include "Scene/MouseRay.h"

namespace GameLogic
{

	using TransformComponent = Yugo::TransformComponent;
	using MeshComponent = Yugo::MeshComponent;
	using AnimationComponent = Yugo::AnimationComponent;
	using EntityTagComponent = Yugo::EntityTagComponent;
	using SpriteComponent = Yugo::SpriteComponent;
	using ScriptComponent = Yugo::ScriptComponent;
	using CameraComponent = Yugo::CameraComponent;

	using Event = Yugo::Event;
	using EventType = Yugo::EventType;
	using MouseButtonPress = Yugo::MouseButtonPress;
	using KeyboardKeyPress = Yugo::KeyboardKeyPress;
	using KeyCode = Yugo::KeyCode;
	using MouseCode = Yugo::MouseCode;

	using KeyCode = Yugo::KeyCode;
	using MouseCode = Yugo::MouseCode;

}