#pragma once
#include <string>
#include <glm/glm.hpp>
namespace GodEngine {
	class shader {
	public:
		shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~shader();

		void Bind() const;
		void UnBind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID{};
	};
}
