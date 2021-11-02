#include "Camera.h"

Camera::Camera()
	: m_ViewMatrix(1.0f), m_AspectRatio((1280.0f/720.0f) * 0.5f)
{
	m_PerspectiveMatrix = glm::perspective(glm::radians(45.0f), m_AspectRatio, 0.1f, 100.0f);
	m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(0.0f, 0.0f, -10.0f));
}

void Camera::UpdateAspectRatio(float r)
{
	m_AspectRatio = r;
	m_PerspectiveMatrix = glm::perspective(glm::radians(45.0f), m_AspectRatio, 0.1f, 100.0f);
}
