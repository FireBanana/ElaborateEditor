#include "Camera.h"

Camera::Camera()
	: m_ViewMatrix(1.0f),
	m_AspectRatio((1280.0f / 720.0f) * 0.5f),
	m_CameraPosition(glm::vec3(0.0f, 0.0f, -10.0f))
{
	m_PerspectiveMatrix = glm::perspective(glm::radians(45.0f), m_AspectRatio, 0.1f, 100.0f);

	m_ViewMatrix = glm::lookAt(
		m_CameraPosition,
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
	double _z = glm::sin(x) * glm::cos(y) * 10.0f;
	double _x = glm::cos(x) * glm::cos(y) * 10.0f;
	double _y = glm::sin(y) * 10.0f;

	m_ViewMatrix = glm::lookAt
	(
		glm::vec3(_x, _y, _z),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	/*m_ViewMatrix = glm::rotate(
		m_ViewMatrix,
		(float)(x * deltaTime),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);*/
}
