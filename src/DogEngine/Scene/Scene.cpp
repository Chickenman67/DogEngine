#include "gepch.h"
#include "Scene.h"
#include "DogEngine/Renderer/Renderer2D.h"
#include "DogEngine/Scene/Component.h"
#include <glm/glm.hpp>

namespace DogEngine {

	Scene::Scene() {}
	Scene::~Scene() {}

	Entity Scene::CreateEntity(const std::string& name) {
		return CreateEntityWithID(UUID(), name);
	}

	Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name) {
		Entity entity = Entity(m_Registry.create(), &m_Registry);
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		if (!name.empty())
			entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts) {
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto& transform = group.get<TransformComponent>(entity);
			auto& sprite = group.get<SpriteRendererComponent>(entity);
			if (sprite.Texture)
				Renderer2D::DrawQuad(glm::vec3(transform.Position.x, transform.Position.y, 0.0f), glm::vec2(transform.Scale.x, transform.Scale.y), sprite.Texture, sprite.TilingFactor, sprite.Color);
			else
				Renderer2D::DrawQuad(glm::vec3(transform.Position.x, transform.Position.y, 0.0f), glm::vec2(transform.Scale.x, transform.Scale.y), sprite.Color);
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.FixedAspectRatio)
				camera.Size = (float)width / (float)height;
		}
	}

	Entity Scene::GetPrimaryCameraEntity() {
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity(entity, &m_Registry);
		}
		return {};
	}

	void Scene::CopyTo(Ref<Scene>& other) {
		GE_CORE_ASSERT(false, "Scene::CopyTo not implemented for this EnTT version");
	}

	Ref<Scene> Scene::Copy() {
		Ref<Scene> newScene = CreateRef<Scene>();
		// TODO: Implement scene copy for this EnTT version
		return newScene;
	}

}
