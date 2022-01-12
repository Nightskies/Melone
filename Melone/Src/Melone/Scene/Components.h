#pragma once

#include <glm/glm.hpp>

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
}
