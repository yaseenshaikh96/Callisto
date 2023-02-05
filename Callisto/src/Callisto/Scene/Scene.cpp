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
	
	void Scene::OnUpdate(TimeStep timeStep)
	{
		// script update
		{
			m_Registry.view<NativeScriptComponent>().each(
				[=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.InstantiateFunction();
						nsc.Instance->m_Entity = Entity(entity, this);
						nsc.OnCreateFunction(nsc.Instance);
					}
					nsc.OnUpdateFunction(nsc.Instance, timeStep);
				}
			);
		}

		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = m_Registry.group<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &(camera.Camera);
					cameraTransform = &(transform.Transform);
					break;
				}

			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);
			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
			
				Renderer2D::DrawQuad(transform.Transform, sprite.Color);
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
		auto tag = entity.AddComponent<TagComponent>(name);
		tag.Tag = name.empty() ? "Entity" : name;
		
		entity.AddComponent<TransformComponent>();
		return entity;
	}

}