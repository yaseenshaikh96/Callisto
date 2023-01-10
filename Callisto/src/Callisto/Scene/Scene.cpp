#include "CallistoPCH.h"
#include "Scene.h"

#include <glm/glm.hpp>

namespace Callisto
{
	Scene::Scene()
	{
		struct MeshComponent{};
		struct TransformComponent
		{
			glm::mat4 Transform;

			operator const glm::mat4&() { return Transform; }
			TransformComponent() = default;
			TransformComponent(const TransformComponent& other) = default;
			TransformComponent(const glm::mat4& transform)
				: Transform(transform)
			{}

		};

		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity);

		if(m_Registry.all_of<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
	
		auto view = m_Registry.view<TransformComponent>();
		for (auto ennt : view)
		{
			
		}
		/*
		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto ennt : group)
		{
			auto&[trans, mesh] = group.get<TransformComponent, MeshComponent>(ennt);
		}
		*/
		
	}
	Scene::~Scene()
	{
	}
	
}