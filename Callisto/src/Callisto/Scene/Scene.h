#pragma once

#include <entt.hpp>

namespace Callisto
{
	class Scene
	{
	public:
		Scene();
		~Scene();
	private:
		entt::registry m_Registry;
	};
}