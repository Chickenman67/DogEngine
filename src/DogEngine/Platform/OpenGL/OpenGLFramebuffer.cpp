#include "gepch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace DogEngine {

	static const uint32_t s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec) {
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate() {
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		// Color attachments
		bool multisample = m_Specification.Samples > 1;
		std::vector<GLenum> drawBuffers;
		if (m_Specification.Attachments.size()) {
			for (size_t i = 0; i < m_Specification.Attachments.size(); i++) {
				uint32_t format = m_Specification.Attachments[i];

				if (format != 0x88F0) { // Not DEPTH24_STENCIL8 — it's a color attachment
					uint32_t textureID;
					glCreateTextures(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 1, &textureID);

					if (multisample) {
						glTextureStorage2DMultisample(textureID, m_Specification.Samples, format, m_Specification.Width, m_Specification.Height, GL_FALSE);
					} else {
						glTextureStorage2D(textureID, 1, format, m_Specification.Width, m_Specification.Height);
						glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					}

					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i,
						multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, textureID, 0);
					m_ColorAttachments.push_back(textureID);
					drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + (GLenum)i);
				}
			}
		}

		// Tell OpenGL which color attachments to render into
		if (drawBuffers.size() > 0)
			glDrawBuffers((GLsizei)drawBuffers.size(), drawBuffers.data());
		else
			glDrawBuffers(0, nullptr); // No color buffer — only depth

		// Depth/stencil attachment
		uint32_t depthStencil = 0;
		glCreateTextures(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, 1, &depthStencil);
		if (multisample) {
			glTextureStorage2DMultisample(depthStencil, m_Specification.Samples, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, GL_FALSE);
		} else {
			glTextureStorage2D(depthStencil, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, depthStencil, 0);
		m_DepthAttachment = depthStencil;

		GE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
			GE_CORE_WARN("Framebuffer resize attempted to invalid size ({0}, {1})", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
		GE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of range!");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
		GE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of range!");
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, GL_RED_INTEGER, GL_INT, &value);
	}

}
