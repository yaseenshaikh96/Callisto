#pragma once

#include <glm/glm.hpp>

namespace Callisto
{
	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		operator const glm::mat4& () { return Transform; }
		operator glm::mat4& () { return Transform; }

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform)
		{}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color)
		{}
	};
}