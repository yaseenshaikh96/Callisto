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
	private:
		Entity m_Entity;
		friend class Scene;
	};
}