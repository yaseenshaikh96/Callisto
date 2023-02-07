#pragma once

#include "Callisto/Scene/Entity.h"

namespace Callisto
{
	class ScriptableEntity
	{
	public:
		template<typename t_Type>
		t_Type& GetComponent()
		{
			return m_Entity.GetComponent<t_Type>();
		}

		virtual void OnCreate() {}
		virtual void OnUpdate(TimeStep timeStep) {}
		virtual void OnDestroy() {}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}