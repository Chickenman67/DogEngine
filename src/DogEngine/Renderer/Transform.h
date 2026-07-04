#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DogEngine {

	struct Transform {
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f }; // Euler angles in degrees
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransform() const {
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position);
			transform = glm::rotate(transform, glm::radians(Rotation.x), { 1.0f, 0.0f, 0.0f });
			transform = glm::rotate(transform, glm::radians(Rotation.y), { 0.0f, 1.0f, 0.0f });
			transform = glm::rotate(transform, glm::radians(Rotation.z), { 0.0f, 0.0f, 1.0f });
			transform = glm::scale(transform, Scale);
			return transform;
		}
	};

}
