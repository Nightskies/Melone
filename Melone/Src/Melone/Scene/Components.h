#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent(void) = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: 
			Translation(translation) 
		{}

		glm::mat4 getTransform(void) const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
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
