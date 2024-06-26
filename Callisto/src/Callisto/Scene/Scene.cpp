#include "CallistoPCH.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "Callisto/Scene/Entity.h"
#include "Callisto/Scene/ScriptableEntity.h"
#include "Callisto/Renderer/Renderer2D.h"

namespace Callisto
{
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
	}
	void Scene::OnUpdateEditor(TimeStep timeStep, const EditorCamera& editorCamera)
	{

		Renderer2D::BeginScene(editorCamera);
		auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : view)
		{
			auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)(entity));
		}
		Renderer2D::EndScene();
	}
	void Scene::OnUpdateRuntime(TimeStep timeStep)
	{
		// script update
		{
			m_Registry.view<NativeScriptComponent>().each(
				[=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity(entity, this);
						nsc.Instance->OnCreate();
					}
					nsc.Instance->OnUpdate(timeStep);
				}
			);
		}

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto group = m_Registry.group<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &(camera.Camera);
					cameraTransform = transform.GetTransform();
					break;
				}

			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);
			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
			
				//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewPortResize(uint32_t width, uint32_t height)
	{
		m_ViewPortWidth = width; 
		m_ViewPortHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& currentCameraComponent = view.get<CameraComponent>(entity);
			if (!currentCameraComponent.FixedAspectRatio)
			{
				currentCameraComponent.Camera.SetViewPortSize(width, height);
			}
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		auto& tag = entity.AddComponent<TagComponent>(name);
		tag.Tag = name.empty() ? "Entity" : name;
		
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (entt::entity enttEntity : view)
		{
			const auto& cc = view.get<CameraComponent>(enttEntity);
			if (cc.Primary)
			{
				return Entity(enttEntity, this);
			}
		}
		return Entity();
	}

	/********************************************************************************************************************************************************/
	template<typename t_Type>
	void Scene::OnComponentAdded(Entity entity, t_Type& component)
	{
		//static_assert(false);
	}
	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewPortSize(m_ViewPortWidth, m_ViewPortHeight);
	}
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}