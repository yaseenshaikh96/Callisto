#include "CallistoPCH.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "Callisto/Scene/Entity.h"
#include "Callisto/Renderer/Renderer2D.h"

namespace Callisto
{
	Scene::Scene()
	{
		/*
		struct MeshComponent
		{
			MeshComponent() = default;
			float x, y, z;
		};
		struct TransformComponent
		{


		};

		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity);

		if(m_Registry.all_of<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
	
		auto view = m_Registry.view<TransformComponent>();
		for (auto ennt : view)
		{
			
		}
		*/
		/*
		*/
		//auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		//for (auto ennt : group)
		//{
		//	auto&[trans, mesh] = group.get<TransformComponent, MeshComponent>(ennt);
		//}
		
	}
	Scene::~Scene()
	{
	}
	
	void Scene::OnUpdate(TimeStep timeStep)
	{
		auto group= m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			
			Renderer2D::DrawQuad(transform.Transform, sprite.Color);
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		auto tag = entity.AddComponent<TagComponent>(name);
		tag.Tag = name.empty() ? "Entity" : name;
		
		entity.AddComponent<TransformComponent>();
		return entity;
	}

}