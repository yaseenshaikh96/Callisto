#pragma once

#include <entt.hpp>

#include "Callisto/Scene/Scene.h"

namespace Callisto
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename t_Type>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<t_Type>(m_EntityHandle);
		}

		template<typename t_Type, typename... Args>
		t_Type& AddComponent(Args&&... args)
		{
			CALLISTO_CORE_ASSERT(!HasComponent<t_Type>(), "Entity already has Component!");
			return m_Scene->m_Registry.emplace<t_Type>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename t_Type>
		t_Type& GetComponent()
		{
			CALLISTO_CORE_ASSERT(HasComponent<t_Type>(), "Entity does not have Component!");
			return m_Scene->m_Registry.get<t_Type>(m_EntityHandle);
		}

		template<typename t_Type>
		void RemoveComponent()
		{
			CALLISTO_CORE_ASSERT(HasComponent<t_Type>(), "Entity does not have Component!");
			return m_Scene->m_Registry.remove<t_Type>(m_EntityHandle);
		}
		operator bool() const { return (uint32_t)m_EntityHandle != 0; }

	private:
		Scene* m_Scene = nullptr;
		entt::entity m_EntityHandle{ 0 };
	};
}