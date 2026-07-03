#include "gepch.h"
#include "OrthoCamera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace GodEngine {
	OrthoCamera::OrthoCamera(float left, float right, float top, float bottom) : m_ProjectionMatrix(glm::ortho(left,right,bottom,top,-1.0f,1.0f))
	{
		RecalculateViewMatrix();
	}
	void OrthoCamera::SetProjection(float left, float right, float top, float bottom)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		RecalculateViewMatrix();
	}
	void OrthoCamera::RecalculateViewMatrix(){
		glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1)) *
			glm::translate(glm::mat4(1.0f), m_Position);

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}
}
