#pragma once

#include "DogEngine/Core.h"
#include "DogEngine/Renderer/Transform.h"
#include "DogEngine/Renderer/Texture.h"
#include "DogEngine/Scene/UUID.h"
#include <glm/glm.hpp>

namespace DogEngine {

	struct TagComponent {
		std::string Tag;
		TagComponent() = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent {
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const DogEngine::Transform& transform)
			: Position(transform.Position), Rotation(transform.Rotation), Scale(transform.Scale) {}
	};

	struct SpriteRendererComponent {
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CircleRendererComponent {
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CameraComponent {
		float Size = 10.0f;
		float NearClip = -1.0f;
		float FarClip = 1.0f;
		bool FixedAspectRatio = false;
		bool Primary = false;

		CameraComponent() = default;
	};

	struct RigidBody2DComponent {
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Runtime
		void* RuntimeBody = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(BodyType type) : Type(type) {}
	};

	struct BoxCollider2DComponent {
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
	};

	struct CircleCollider2DComponent {
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Runtime
		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
	};

	struct IDComponent {
		UUID ID;
		IDComponent() = default;
		IDComponent(const UUID& id) : ID(id) {}
	};

}
