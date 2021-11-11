#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();

	inline const glm::mat4& GetPerspective() { return m_PerspectiveMatrix; }
	inline const glm::mat4& GetView() { return m_ViewMatrix; }

	void UpdateAspectRatio(float r);
	void Rotate(double x, double y, double deltaTime);

private:
	glm::mat4 m_PerspectiveMatrix;
	glm::mat4 m_ViewMatrix;

	float m_AspectRatio;
	float m_X, m_Y;
	float m_Distance;
	float m_Sensitivity;
};