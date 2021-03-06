#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "SceneCamera.h"

#include "ScriptableEntity.h"

namespace Melone
{
	class TagComponent
	{
	public:
		std::string Tag = "Entity";

		TagComponent() = default;
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

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: 
			Translation(translation) 
		{}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

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

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	class NativeScriptComponent
	{
	public:
		SPtr<ScriptableEntity> Instance;

		std::function<SPtr<ScriptableEntity>()> InstantiateScript;
		std::function<void()> DestroyScript;

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return std::make_shared<T>(); };
			DestroyScript = [&]() { Instance.reset(); };
		}
	};
}
