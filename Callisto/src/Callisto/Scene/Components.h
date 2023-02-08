#pragma once

#include <glm/glm.hpp>

#include "Callisto/Renderer/Camera.h"
#include "Callisto/Scene/SceneCamera.h"
#include "Callisto/Core/TimeStep.h"

namespace Callisto
{
	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag)
			: Tag(tag)
		{}
		std::string Tag;
	};

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

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
	};

	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<ScriptableEntity*()> InstantiateScript;
		std::function<void(NativeScriptComponent*)> DestroyScript;

		template<typename t_Type>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new t_Type()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}