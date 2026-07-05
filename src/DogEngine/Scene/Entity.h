#pragma once

#include "DogEngine/Core.h"
#include "DogEngine/Scene/Component.h"

#include <entt/entt.hpp>

namespace DogEngine {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, entt::registry* registry);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			GE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Registry->emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() {
			GE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Registry->get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() {
			return m_Registry->any_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent() {
			GE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Registry->remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Registry == other.m_Registry; }
		bool operator!=(const Entity& other) const { return !(*this == other); }
	private:
		entt::entity m_EntityHandle{ entt::null };
		entt::registry* m_Registry = nullptr;
	};

}
