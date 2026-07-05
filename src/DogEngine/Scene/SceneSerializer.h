#pragma once

#include "DogEngine/Core.h"
#include "DogEngine/Scene/Scene.h"
#include <yaml-cpp/yaml.h>

namespace DogEngine {

	class SceneSerializer {
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};

}
