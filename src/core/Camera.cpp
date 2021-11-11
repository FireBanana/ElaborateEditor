#include "Camera.h"
#include "Logger.h"

Camera::Camera()
	: m_ViewMatrix(1.0f),
	m_AspectRatio((1280.0f / 720.0f) * 0.5f),
	m_Sensitivity(3.0f), m_Distance(10.0f), m_X(0.0f), m_Y(0.0f)
{
	m_PerspectiveMatrix = glm::perspective(glm::radians(45.0f), m_AspectRatio, 0.1f, 100.0f);

	m_ViewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -m_Distance),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::UpdateAspectRatio(float r)
{
	m_AspectRatio = r;
	m_PerspectiveMatrix = glm::perspective(glm::radians(45.0f), m_AspectRatio, 0.1f, 100.0f);
}

void Camera::Rotate(double x, double y, double deltaTime)
{
	m_X += x * deltaTime * m_Sensitivity;
	m_Y += y * deltaTime * m_Sensitivity;

	double _z = glm::sin(m_X) * glm::cos(m_Y) * m_Distance;
	double _x = glm::cos(m_X) * glm::cos(m_Y) * m_Distance;
	double _y = glm::sin(m_Y) * m_Distance;
	
	m_ViewMatrix = glm::lookAt
	(
		glm::vec3(_x, _y, _z),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, glm::cos(m_Y) >= 0 ? 1.0f : -1.0f, 0.0f)
	);
}
