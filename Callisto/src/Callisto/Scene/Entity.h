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
			t_Type& component = m_Scene->m_Registry.emplace<t_Type>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<t_Type>(*this, component);
			return component;
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
			m_Scene->m_Registry.remove<t_Type>(m_EntityHandle);
		}


		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(const Entity& other) const 
		{ 
			return m_EntityHandle == other.m_EntityHandle
				&& m_Scene == other.m_Scene; 
		}
		bool operator!=(const Entity& other) const
		{
			return !(operator==(other));
		}

	private:
		Scene* m_Scene = nullptr;
		entt::entity m_EntityHandle{ entt::null };
	};
}