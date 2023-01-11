#pragma once

#include <entt.hpp>

#include "Callisto/Scene/Components.h"
#include "Callisto/Core/TimeStep.h"


namespace Callisto
{
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();
		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(TimeStep timeStep);

	private:
		entt::registry m_Registry;
		
		friend class Entity;
	};
}