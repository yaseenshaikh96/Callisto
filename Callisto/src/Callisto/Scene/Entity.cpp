#include "CallistoPCH.h"
#include "Entity.h"

namespace Callisto
{
	Entity::Entity(entt::entity entityHandle, Scene* scene)
		: m_EntityHandle(entityHandle), m_Scene(scene)
	{}
}