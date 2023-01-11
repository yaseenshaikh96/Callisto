#pragma once

#include <entt.hpp>

#include "Callisto/Scene/Components.h"
#include "Callisto/Core/TimeStep.h"

namespace Callisto
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		void OnUpdate(TimeStep timeStep);

		entt::registry& GetRegistry() { return m_Registry; }

		entt::entity CreateEntity();

	private:
		entt::registry m_Registry;
	};
}