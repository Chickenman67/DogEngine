#include "gepch.h"
#include "Entity.h"

namespace DogEngine {

	Entity::Entity(entt::entity handle, entt::registry* registry)
		: m_EntityHandle(handle), m_Registry(registry) {}

}
