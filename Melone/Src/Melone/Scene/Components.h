#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Melone
{
	class TagComponent
	{
	public:
		std::string Tag;

		TagComponent(void) = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: 
			Tag(tag) 
		{}
	};

	class TransformComponent
	{
	public:
		glm::mat4 Transform{ 1.0f };

		TransformComponent(void) = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 & transform)
			: 
			Transform(transform) 
		{}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	class SpriteRendererComponent
	{
	public:
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: 
			Color(color) 
		{}
	};

	class CameraComponent
	{
	public:
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent(void) = default;
		CameraComponent(const CameraComponent&) = default;
	};

	class NativeScriptComponent
	{
	public:
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind(void)
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}
