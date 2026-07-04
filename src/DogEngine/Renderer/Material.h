#pragma once

#include "DogEngine/Core.h"
#include "DogEngine/Renderer/Shader.h"
#include "DogEngine/Renderer/Texture.h"
#include <unordered_map>
#include <glm/glm.hpp>

namespace DogEngine {

	class Material {
	public:
		Material(const Ref<Shader>& shader)
			: m_Shader(shader) {}

		void SetShader(const Ref<Shader>& shader) { m_Shader = shader; }
		const Ref<Shader>& GetShader() const { return m_Shader; }

		void SetFloat(const std::string& name, float value) { m_FloatUniforms[name] = value; }
		void SetFloat3(const std::string& name, const glm::vec3& value) { m_Float3Uniforms[name] = value; }
		void SetFloat4(const std::string& name, const glm::vec4& value) { m_Float4Uniforms[name] = value; }
		void SetInt(const std::string& name, int value) { m_IntUniforms[name] = value; }
		void SetTexture(const std::string& name, const Ref<Texture2D>& texture, uint32_t slot) { 
			m_TextureUniforms[name] = { texture, slot };
		}

		void Apply() {
			m_Shader->Bind();
			for (auto& [name, value] : m_FloatUniforms)
				m_Shader->UploadUniformFloat(name, value);
			for (auto& [name, value] : m_Float3Uniforms)
				m_Shader->UploadUniformFloat3(name, value);
			for (auto& [name, value] : m_Float4Uniforms)
				m_Shader->UploadUniformFloat4(name, value);
			for (auto& [name, value] : m_IntUniforms)
				m_Shader->UploadUniformInt(name, value);
			for (auto& [name, texInfo] : m_TextureUniforms) {
				texInfo.first->Bind(texInfo.second);
				m_Shader->UploadUniformInt(name, (int)texInfo.second);
			}
		}

	private:
		Ref<Shader> m_Shader;
		std::unordered_map<std::string, float> m_FloatUniforms;
		std::unordered_map<std::string, glm::vec3> m_Float3Uniforms;
		std::unordered_map<std::string, glm::vec4> m_Float4Uniforms;
		std::unordered_map<std::string, int> m_IntUniforms;
		std::unordered_map<std::string, std::pair<Ref<Texture2D>, uint32_t>> m_TextureUniforms;
	};

}
